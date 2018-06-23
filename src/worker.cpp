#include "../include/worker.h"

Worker::Worker() {
  this->source = "";
  this->target = "";
  this->filesCount = 0;
  this->directoriesCount = 0;
  this->totalCount = 0;
  this->size = 0;
  this->processedCount = 0;
  this->processedSize = 0;
  this->copyBufferSize = DEFAULT_COPY_BUFFER_SIZE;
  this->synchronize = false;
  this->keepObsolete = false;
  this->errorOccurred = false;
  this->criterion = DEFAULT_COMPARISON_CRITERION;
  this->log = nullptr;
  this->progress = true;
}

bool Worker::isCurrentSuperiorThanBackedUp(const QString &currentPath, const QString &backedUpPath) const {
  QFileInfo current(currentPath),
            backedUp(backedUpPath);

  if(this->criterion == CRITERION_BIGGER) {
    return (current.size() > backedUp.size());
  }

  return (current.lastModified() > backedUp.lastModified());
}

bool Worker::transferFileAttributes(const QString &sourcePath, const QString &destinationPath) {
  QFileInfo s(sourcePath);
  QFile source(sourcePath),
        destination(destinationPath);
  bool succeeded = true;

  if(!s.isDir()) {
#if defined Q_OS_LINUX
    struct utimbuf time;

    time.actime = s.lastRead().toSecsSinceEpoch();
    time.modtime = s.lastModified().toSecsSinceEpoch();

    if(utime(destination.fileName().toStdString().c_str(), &time) == -1) {
      this->log->logError(tr("Failed to transfer time information from '%1' to '%2'!").arg(sourcePath, destinationPath));
      succeeded = false;
    } else {
      this->log->logEvent(tr("Successfully transferred time information from '%1' to '%2'!").arg(sourcePath, destinationPath));
    }
#elif defined Q_OS_WIN
    HANDLE hSource = CreateFileW(sourcePath.toStdWString().c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hSource == INVALID_HANDLE_VALUE) {
      this->log->logError(tr("Failed to open '%1' for reading! (error code: 0x%2)").arg(sourcePath, QString::number(GetLastError(), 16)));
      succeeded = false;
    }

    HANDLE hDestination = CreateFileW(destinationPath.toStdWString().c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hDestination == INVALID_HANDLE_VALUE) {
      this->log->logError(tr("Failed to open '%1' for reading! (error code: 0x%2)").arg(destinationPath, QString::number(GetLastError(), 16)));
      succeeded = false;
    }

    FILETIME creationTime, lastAccessTime, lastModificationTime;

    if(!GetFileTime(hSource, &creationTime, &lastAccessTime, &lastModificationTime)) {
      this->log->logError(tr("Failed to read time information from file '%1' (error code: 0x%2)!").arg(sourcePath, QString::number(GetLastError(), 16)));
      succeeded = false;
    } else if(!SetFileTime(hDestination, &creationTime, &lastAccessTime, &lastModificationTime)) {
      this->log->logError(tr("Failed to transfer time information to file '%1' (error code: 0x%2)!").arg(destinationPath, QString::number(GetLastError(), 16)));
      succeeded = false;
    } else {
      this->log->logEvent(tr("Successfully transferred time information from '%1' to '%2'!").arg(sourcePath, destinationPath));
    }

    CloseHandle(hSource);
    CloseHandle(hDestination);
#else
#error "Unsupported operating system!"
#endif
  }

#if defined Q_OS_WIN
  DWORD attributes = GetFileAttributesW(source.fileName().toStdWString().c_str());

  if(attributes != INVALID_FILE_ATTRIBUTES) {
    this->log->logEvent(tr("Attributes read successfully from file '%1'.").arg(sourcePath));
    if(!SetFileAttributesW(destination.fileName().toStdWString().c_str(), attributes)) {
      this->log->logError(tr("Failed to set attributes for file '%1'!").arg(destinationPath));
      succeeded = false;
    } else {
      this->log->logEvent(tr("Attributes set successfully for file '%1'.").arg(destinationPath));
    }
  } else {
    this->log->logError(tr("Failed to read attributes from file '%1'!").arg(sourcePath));
    succeeded = false;
  }
#endif

  if(destination.setPermissions(source.permissions())) {
    this->log->logEvent(tr("Permissions set successfully for file '%1'.").arg(destinationPath));
  } else {
    this->log->logError(tr("Failed to set permissions for file '%1'!").arg(destinationPath));
    succeeded = false;
  }

  return succeeded;
}

bool Worker::copyFile(const QString &sourcePath, const QString &destinationPath, qint64 size) {
  QFile source(sourcePath),
        destination(destinationPath);
  char * bytes = new char[this->copyBufferSize];
  quint64 actuallyWritten = 0, toBeWritten = 0, totalWritten = 0;
  bool succeeded = true;

  if(source.open(QIODevice::ReadOnly)) {
    this->log->logEvent(tr("Successfully opened file '%1' for copying.").arg(sourcePath));
  } else {
    this->log->logError(tr("Failed to open file '%1' for copying: %2!").arg(sourcePath, source.errorString()));
  }

  if(destination.open(QIODevice::WriteOnly))
    this->log->logEvent(tr("Successfully opened file '%1' for copying.").arg(destinationPath));
  else
    this->log->logError(tr("Failed to open file '%1' for copying: %2!").arg(destinationPath).arg(destination.errorString()));

  if(source.isReadable() && destination.isWritable()) {
    while((toBeWritten = source.read(bytes, this->copyBufferSize)) != 0) {
      if(toBeWritten < 0) {
        goto error;
      }

      if((actuallyWritten = destination.write(bytes, toBeWritten)) != toBeWritten) {
        goto error;
      }

      totalWritten += actuallyWritten;
      this->processedSize += actuallyWritten;

      emit this->triggerCurrentProgress((int) ceil(totalWritten * 100 / size));
      emit this->triggerOverallProgress((int) ceil(this->processedSize * 100 / this->size));

      memset(bytes, 0, this->copyBufferSize);
    }

    if(this->transferFileAttributes(sourcePath, destinationPath)) {
      this->log->logEvent(tr("Successfully transfered file attributes to '%1'.").arg(destinationPath));
    } else {
      this->log->logError(tr("Failed to transfer file attributes to '%1'!").arg(sourcePath));
    }

    this->log->logEvent(tr("Successfully copied file '%1' to '%2'.").arg(sourcePath, destinationPath));

    goto clean;
  }

  error:
  succeeded = false;
  this->log->logError(tr("Failed to copy file '%1' to '%2'.").arg(sourcePath).arg(destinationPath));

  clean:
  delete[] bytes;
  source.close();
  destination.close();

  return succeeded;
}

Worker::~Worker() {
  delete this->log;
}

qint64 Worker::getFilesCount() const {
  return this->filesCount;
}

qint64 Worker::getDirectoriesCount() const {
  return this->directoriesCount;
}

qint64 Worker::getSize() const {
  return this->size;
}

qint64 Worker::getProcessedCount() const {
  return this->processedCount;
}

bool Worker::getErrorOccurred() const {
  return this->errorOccurred;
}

bool Worker::getProgress() const {
  return this->progress;
}

void Worker::setSource(const QString &source) {
  this->source = QString(source);
}

void Worker::setTarget(const QString &target) {
  this->target = QString(target);
}

void Worker::setSynchronize(bool synchronize) {
  this->synchronize = synchronize;
}

void Worker::setKeepObsolete(bool keepObsolete) {
  this->keepObsolete = keepObsolete;
}

void Worker::setCriterion(Criterion criterion) {
  this->criterion = criterion;
}

void Worker::setCopyBufferSize(qint64 copyBufferSize) {
  this->copyBufferSize = MEGABYTE * copyBufferSize;
}

void Worker::setProgress(bool progress) {
  this->progress = progress;

  if(!this->progress) {
    this->log->logEvent("Backup process cancellation has been requested.");
  }
}

void Worker::reinitializeCounters() {
  this->filesCount = 0;
  this->directoriesCount = 0;
  this->size = 0;
  this->totalCount = 0;
  this->processedCount = 0;
  this->processedSize = 0;
}

void Worker::createLogsAt(const QString &path) {
  this->log = new Logger(path, WORKER_EVENT_LOG_FILE_NAME, WORKER_ERROR_LOG_FILE_NAME);
}

void Worker::work() {
  emit this->started();

  QDirIterator i(this->source, WORKER_ITEM_FILTERS, QDirIterator::Subdirectories);
  QDir root(this->source);

  while(i.hasNext() && this->progress) {
    QString sourcePath = i.next();
    bool isSystem = false;

#if defined Q_OS_WIN
    DWORD attributes = GetFileAttributesW(sourcePath.toStdWString().c_str());

    if(attributes == INVALID_FILE_ATTRIBUTES) {
      this->log->logError(tr("Unable to get attributes of file or directory '%1'!").arg(sourcePath));
    }

    isSystem = attributes & FILE_ATTRIBUTE_SYSTEM;
    isSystem |= sourcePath.startsWith(this->source + "$RECYCLE.BIN");
#endif

    if(!isSystem) {
      QString destinationPath = this->target + '/' + root.relativeFilePath(sourcePath);

      QFileInfo sourceFileInfo(sourcePath);

      emit this->triggerCurrentItem(sourceFileInfo.baseName());

      if(this->synchronize) {
        if(!sourceFileInfo.isDir()) {
          if(sourceFileInfo.exists()) {
            if(this->isCurrentSuperiorThanBackedUp(sourcePath, destinationPath)) {
              emit this->triggerCurrentOperation(tr("Removing older version of file"));

              if(QFile::remove(destinationPath)) {
                this->log->logEvent(tr("Successfully removed older version of a file at '%1'.").arg(destinationPath));
              } else {
                this->errorOccurred = true;
                this->log->logError(tr("Failed to remove older version of a file at '%1'.").arg(destinationPath));
              }

              goto copying;
            } else {
              goto skipping;
            }
          } else {
            goto copying;
          }
        } else if(!sourceFileInfo.exists()) {
          goto cloning;
        } else {
          goto skipping;
        }
      } else {
        if(!sourceFileInfo.isDir()) {
          goto copying;
        } else {
          goto cloning;
        }
      }

      copying:
      emit this->triggerCurrentOperation(tr("Copying file"));

      if(!this->copyFile(sourcePath, destinationPath, sourceFileInfo.size())) {
        this->errorOccurred = true;
      }

      goto done;

      cloning:
      emit this->triggerCurrentOperation(tr("Creating directory"));

      if(root.mkdir(destinationPath) && this->transferFileAttributes(sourcePath, destinationPath)) {
        this->log->logEvent(tr("Successfully recreated directory '%1' as '%2'.").arg(sourcePath, destinationPath));
      } else {
        this->errorOccurred = true;
        this->log->logError(tr("Failed to recreate directory '%1' as '%2'!").arg(sourcePath, destinationPath));
      }

      goto done;

      skipping:
      this->log->logEvent(tr("Skipping file or directory '%1'.").arg(sourcePath));
      emit this->triggerCurrentOperation(tr("Skipping item"));

      done:
      this->processedCount++;
      this->processedSize += sourceFileInfo.size();
    }
  }

  if(this->progress) {
    if(!this->keepObsolete) {
      QDirIterator i(this->target, WORKER_ITEM_FILTERS, QDirIterator::Subdirectories);
      root = QDir(this->target);

      while(i.hasNext()) {
        QString currentFile(i.next());
        QFile correspondingFile(this->source + "/" + root.relativeFilePath(currentFile));
        QFileInfo currentFileInfo(currentFile);

        if(!correspondingFile.exists() && currentFileInfo.exists()) {
          if(currentFileInfo.isDir()) {
            emit this->triggerCurrentOperation(tr("Removing folder"));

            QDir temp(currentFile);

            if(temp.removeRecursively()) {
              this->log->logEvent(tr("Successfully removed obsolete folder '%1'.").arg(currentFile));
            } else {
              this->errorOccurred = true;
              this->log->logError(tr("Failed to remove obsolete folder '%1'!").arg(currentFile));
            }
          } else {
            emit this->triggerCurrentOperation(tr("Removing file"));

            if(QFile::remove(currentFile)) {
              this->log->logEvent(tr("Successfully removed obsolete file '%1'.").arg(currentFile));
            } else {
              this->errorOccurred = true;
              this->log->logError(tr("Failed to remove obsolete file '%1'.").arg(currentFile));
            }
          }
        }
      }
    }
  }

  emit this->finished((int) ACTION_BACKUP);
}

void Worker::analyze() {
  emit this->started();

  this->reinitializeCounters();

  QDirIterator i(this->source, WORKER_ITEM_FILTERS, QDirIterator::Subdirectories);

    while(i.hasNext()) {
      QString currentPath(i.next());
      QFileInfo current(currentPath);
      bool isSystem = false;

#if defined Q_OS_WIN
      DWORD attributes = GetFileAttributesW(currentPath.toStdWString().c_str());

      if(attributes == INVALID_FILE_ATTRIBUTES) {
        this->log->logError(tr("Unable to get attributes of file or directory '%1'!").arg(currentPath));
      }

      isSystem = attributes & FILE_ATTRIBUTE_SYSTEM;
      isSystem |= currentPath.startsWith(this->source + "$RECYCLE.BIN");
#endif

      if(!isSystem) {
        if(current.isDir()) {
          this->directoriesCount++;
        } else {
          this->filesCount++;
        }

        this->totalCount++;
        this->size += current.size();
        emit this->triggerAnalysisProgress(this->filesCount, this->directoriesCount, this->size);
      }
  }

  emit this->finished((int) ACTION_ANALYSIS);
}

void Worker::doWork() {
  this->work();
}
