/*!
 * This file is a part of Backy project, a simple backup creation and
 * maintaining solution.
 * 
 * Copyright (C) 2018 Marek Felsoci <marek.felsoci@gmail.com> (Feldev)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * \file worker.cpp
 * \brief Contains definitions related to Worker class.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0
 */
#include "../include/worker.h"

/*!
 * \fn Worker::Worker()
 * \brief Constructor of Worker class.
 */
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

/*!
 * \fn bool Worker::isCurrentSuperiorThanBackedUp(const QString
 *          &currentPath, const QString &backedUpPath) const
 * \param currentPath Path to source file version.
 * \param backedUpPath Path to target file version.
 * \brief Comapres source and target version of currently processed
 *        item.
 * \returns \c true if source version at \a currentPath is superior to
 *          target version at \a backedUpPath according to selected
 *          comparison cirterion.
 * \returns \c false otherwise.
 */
bool Worker::isCurrentSuperiorThanBackedUp(const QString &currentPath,
  const QString &backedUpPath) const {
  QFileInfo current(currentPath),
            backedUp(backedUpPath);

  if(this->criterion == CRITERION_BIGGER) {
    return (current.size() > backedUp.size());
  }

  return (current.lastModified() > backedUp.lastModified());
}

/*!
 * \fn bool Worker::transferFileAttributes(const QString &sourcePath,
 *     const QString &destinationPath)
 * \param sourcePath Path to source file version.
 * \param destinationPath Path to target file version.
 * \brief Transfers attributes between source and target file versions.
 * \returns \c true if the trasfer has been completed successfully.
 * \returns \c false otherwise.
 */
bool Worker::transferFileAttributes(const QString &sourcePath,
  const QString &destinationPath) {
  QFileInfo s(sourcePath);
  QFile source(sourcePath),
        destination(destinationPath);
  bool succeeded = true;

  /* If the file is not a directory transfer also time information */
  if(!s.isDir()) {
#if defined Q_OS_LINUX
    struct utimbuf time;

    time.actime = s.lastRead().toSecsSinceEpoch();
    time.modtime = s.lastModified().toSecsSinceEpoch();

    if(utime(destination.fileName().toStdString().c_str(),
      &time) == -1) {
      this->log->logError(tr("Failed to transfer time information from"
      " '%1' to '%2'!").arg(sourcePath, destinationPath));
      succeeded = false;
    } else {
      this->log->logEvent(tr("Successfully transferred time information"
      " from '%1' to '%2'!").arg(sourcePath, destinationPath));
    }
#elif defined Q_OS_WIN
    HANDLE hSource = CreateFileW(sourcePath.toStdWString().c_str(),
      GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE |
      FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
      NULL);

    if(hSource == INVALID_HANDLE_VALUE) {
      this->log->logError(tr("Failed to open '%1' for reading! (error"
        " code: 0x%2)").arg(sourcePath, QString::number(GetLastError(),
        16)));
      succeeded = false;
    }

    HANDLE hDestination = CreateFileW(
      destinationPath.toStdWString().c_str(), GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hDestination == INVALID_HANDLE_VALUE) {
      this->log->logError(tr("Failed to open '%1' for reading! (error"
        " code: 0x%2)").arg(destinationPath,
        QString::number(GetLastError(), 16)));
      succeeded = false;
    }

    FILETIME creationTime, lastAccessTime, lastModificationTime;

    if(!GetFileTime(hSource, &creationTime, &lastAccessTime,
      &lastModificationTime)) {
      this->log->logError(tr("Failed to read time information from file"
      " '%1' (error code: 0x%2)!").arg(sourcePath,
      QString::number(GetLastError(), 16)));
      succeeded = false;
    } else if(!SetFileTime(hDestination, &creationTime, &lastAccessTime,
      &lastModificationTime)) {
      this->log->logError(tr("Failed to transfer time information to"
      " file '%1' (error code: 0x%2)!").arg(destinationPath, QString::number(GetLastError(), 16)));
      succeeded = false;
    } else {
      this->log->logEvent(tr("Successfully transferred time information"
      " from '%1' to '%2'!").arg(sourcePath, destinationPath));
    }

    CloseHandle(hSource);
    CloseHandle(hDestination);
#else
#error Unsupported operating system!
#endif
  }

#if defined Q_OS_WIN
  /* Transfer Windows file system attributes */
  DWORD attributes = GetFileAttributesW(
    source.fileName().toStdWString().c_str());

  if(attributes != INVALID_FILE_ATTRIBUTES) {
    this->log->logEvent(tr("Attributes read successfully from file"
    " '%1'.").arg(sourcePath));
    if(!SetFileAttributesW(
      destination.fileName().toStdWString().c_str(), attributes)) {
      this->log->logError(tr("Failed to set attributes for file"
      " '%1'!").arg(destinationPath));
      succeeded = false;
    } else {
      this->log->logEvent(tr("Attributes set successfully for file"
      " '%1'.").arg(destinationPath));
    }
  } else {
    this->log->logError(tr("Failed to read attributes from file"
    " '%1'!").arg(sourcePath));
    succeeded = false;
  }
#endif

  /* Transfer permissions */
  if(destination.setPermissions(source.permissions())) {
    this->log->logEvent(tr("Permissions set successfully for file"
    " '%1'.").arg(destinationPath));
  } else {
    this->log->logError(tr("Failed to set permissions for file"
    " '%1'!").arg(destinationPath));
    succeeded = false;
  }

  return succeeded;
}

/*!
 * \fn bool Worker::copyFile(const QString &sourcePath,
 *     const QString &destinationPath, qint64 size)
 * \param sourcePath Path to source file version.
 * \param destinationPath Path to target file version.
 * \param size Size of file to be copied.
 * \brief Copies source file version to target file version.
 * \returns \c true if the copy has been successfully completed.
 * \returns \c false otherwise.
 */
bool Worker::copyFile(const QString &sourcePath,
  const QString &destinationPath, qint64 size) {
  QFile source(sourcePath),
        destination(destinationPath);
  qint64 actualBufferSize = size < this->copyBufferSize ? size : this->copyBufferSize;
  char * bytes = new char[actualBufferSize];
  qint64 actuallyWritten = 0, toBeWritten = 0, totalWritten = 0;
  bool succeeded = true;

  if(source.open(QIODevice::ReadOnly)) {
    this->log->logEvent(tr("Successfully opened file '%1' for"
      " copying.").arg(sourcePath));
  } else {
    this->log->logError(tr("Failed to open file '%1' for copying:"
      " %2!").arg(sourcePath, source.errorString()));
  }

  if(destination.open(QIODevice::WriteOnly))
    this->log->logEvent(tr("Successfully opened file '%1' for"
      " copying.").arg(destinationPath));
  else
    this->log->logError(tr("Failed to open file '%1' for"
      " copying: %2!").arg(destinationPath).arg(
      destination.errorString()));

  if(source.isReadable() && destination.isWritable()) {
    while((toBeWritten = source.read(bytes,
      actualBufferSize)) != 0) {
      if(toBeWritten < 0) {
        goto error;
      }

      if((actuallyWritten = destination.write(bytes, toBeWritten))
        != toBeWritten) {
        goto error;
      }

      totalWritten += actuallyWritten;
      this->processedSize += actuallyWritten;

      emit this->triggerCurrentProgress(
        CEILD(totalWritten * 100, size));
      emit this->triggerOverallProgress(
        CEILD(this->processedSize * 100, this->size));

      memset(bytes, 0, actualBufferSize);
    }

    if(this->transferFileAttributes(sourcePath, destinationPath)) {
      this->log->logEvent(tr("Successfully transfered file attributes"
        " to '%1'.").arg(destinationPath));
    } else {
      this->log->logError(tr("Failed to transfer file attributes to"
        " '%1'!").arg(sourcePath));
    }

    this->log->logEvent(tr("Successfully copied file '%1' to"
      " '%2'.").arg(sourcePath, destinationPath));

    goto clean;
  }

  error:
  succeeded = false;
  this->log->logError(tr("Failed to copy file '%1' to"
    " '%2'.").arg(sourcePath).arg(destinationPath));

  clean:
  delete[] bytes;
  source.close();
  destination.close();

  return succeeded;
}

/*!
 * \fn Worker::~Worker()
 * \brief Destructor of Worker class.
 */
Worker::~Worker() {
  delete this->log;
}

/*!
 * \fn qint64 Worker::getFilesCount() const
 * \returns Count of files that have already been discovered during
 *          backup source location analysis.
 */
qint64 Worker::getFilesCount() const {
  return this->filesCount;
}

/*!
 * \fn qint64 Worker::getDirectoriesCount() const
 * \returns Count of directories that have already been discovered
 *          during backup source location analysis.
 */
qint64 Worker::getDirectoriesCount() const {
  return this->directoriesCount;
}

/*!
 * \fn qint64 Worker::getSize() const
 * \returns Total size of entries that have already been discovered
 *          during backup source location analysis.
 */
qint64 Worker::getSize() const {
  return this->size;
}

/*!
 * \fn qint64 Worker::getProcessedCount() const
 * \returns Count of items (files and directories) in backup source
 *          location that have already been processed.
 */
qint64 Worker::getProcessedCount() const {
  return this->processedCount;
}

/*!
 * \fn bool Worker::getErrorOccurred() const
 * \returns Boolean value which indicates whether an error occurred
 *          during backup process.
 */
bool Worker::getErrorOccurred() const {
  return this->errorOccurred;
}

/*!
 * \fn bool Worker::getProgress() const
 * \returns Boolean value which indicates whether the current action
 *          being performed by Worker has to continue or to be aborted.
 */
bool Worker::getProgress() const {
  return this->progress;
}

/*!
 * \fn void Worker::setSource(const QString &source)
 * \param source New path to backup source location.
 * \brief Replaces current path to backup source location by a new one.
 */
void Worker::setSource(const QString &source) {
  this->source = QString(source);
}

/*!
 * \fn void Worker::setTarget(const QString &target)
 * \param target New path to backup target location.
 * \brief Replaces current path to backup target location by a new one.
 */
void Worker::setTarget(const QString &target) {
  this->target = QString(target);
}

/*!
 * \fn void Worker::setSynchronize(bool synchronize)
 * \param synchronize New decision on synchronization.
 * \brief Assigns a new value to the boolean value which decides whether
 *        backup source location should be only synchronized with a
 *        previously created backup in target location.
 */
void Worker::setSynchronize(bool synchronize) {
  this->synchronize = synchronize;
}

/*!
 * \fn void Worker::setKeepObsolete(bool keepObsolete)
 * \param keepObsolete New decision on keeping obsolete file(s) in
 *        backup target location.
 * \brief Assigns a new value to the boolean value deciding whether any
 *        file in backup target location which does not exist in source
 *        location anymore should be kept or deleted.
 */
void Worker::setKeepObsolete(bool keepObsolete) {
  this->keepObsolete = keepObsolete;
}

/*!
 * \fn void Worker::setCriterion(Criterion criterion)
 * \param criterion New criterion.
 * \brief Changes current comparison criterion.
 */
void Worker::setCriterion(Criterion criterion) {
  this->criterion = criterion;
}

/*!
 * \fn void Worker::setCopyBufferSize(qint64 copyBufferSize)
 * \param copyBufferSize New copy buffer size.
 * \brief Changes current copy buffer size.
 */
void Worker::setCopyBufferSize(qint64 copyBufferSize) {
  this->copyBufferSize = MEGABYTE * copyBufferSize;
}

/*!
 * \fn void Worker::setProgress(bool progress)
 * \param progress New decision on continuing the current action being
 *        processed.
 * \brief Assigns a new value to the boolean value which indicates
 *        whether the current action performed by Worker has to continue
 *        or to be aborted.
 */
void Worker::setProgress(bool progress) {
  this->progress = progress;

  if(!this->progress) {
    this->log->logEvent("Backup process cancellation has been"
      " requested.");
  }
}

/*!
 * \fn void Worker::reinitializeCounters()
 * \brief Reinitializes all item counters to 0.
 */
void Worker::reinitializeCounters() {
  this->filesCount = 0;
  this->directoriesCount = 0;
  this->size = 0;
  this->totalCount = 0;
  this->processedCount = 0;
  this->processedSize = 0;
}

/*!
 * \fn void Worker::createLogsAt(const QString &path)
 * \param path Path to log files location.
 * \brief Creates a new Logger instance in order to be able to keep
 *        trace of this Worker instance's activity.
 */
void Worker::createLogsAt(const QString &path) {
  this->log = new Logger(path, WORKER_EVENT_LOG_FILE_NAME,
    WORKER_ERROR_LOG_FILE_NAME);
}

/*!
 * \fn void Worker::work()
 * \brief Performs the backup.
 */
void Worker::work() {
  this->progress = true;
  emit this->started();

  QDirIterator i(this->source, WORKER_ITEM_FILTERS,
    QDirIterator::Subdirectories);
  QDir root(this->source);

  /* Iterate over all entries in the backup source folder */
  while(i.hasNext() && this->progress) {
    QString sourcePath = i.next();
    bool isSystem = false;

    /*
     * Ignore system file(s) and file(s) in the Recycle Bin (Windows) or
     * Trash (Linux)
     */
#if defined Q_OS_WIN
    DWORD attributes = GetFileAttributesW(
      sourcePath.toStdWString().c_str());

    if(attributes == INVALID_FILE_ATTRIBUTES) {
      this->log->logError(tr("Unable to get attributes of file or"
        " directory '%1'!").arg(sourcePath));
    }

    isSystem = attributes & FILE_ATTRIBUTE_SYSTEM;
    isSystem = isSystem || sourcePath.startsWith(this->source +
      "$RECYCLE.BIN", Qt::CaseInsensitive);
#elif defined Q_OS_LINUX
    isSystem = sourcePath.startsWith(".local/share/Trash");
#else
#error Unsupported operating system!
#endif

    if(!isSystem) {
      QString destinationPath = this->target + '/'
        + root.relativeFilePath(sourcePath);

      QFileInfo sourceFileInfo(sourcePath),
                destinationFileInfo(destinationPath);

      emit this->triggerCurrentItem(sourcePath);

      /* Process current entry */
      if(this->synchronize) {
        if(!sourceFileInfo.isDir()) {
          if(destinationFileInfo.exists()) {
            if(this->isCurrentSuperiorThanBackedUp(sourcePath,
              destinationPath)) {
              emit this->triggerCurrentOperation(tr("Removing older"
                " version of file"));

              if(QFile::remove(destinationPath)) {
                this->log->logEvent(tr("Successfully removed older"
                  " version of a file at '%1'.").arg(destinationPath));
                goto copying;
              } else {
                this->errorOccurred = true;
                this->log->logError(tr("Failed to remove older version"
                  " of a file at '%1'.").arg(destinationPath));
              }
            } else {
              goto skipping;
            }
          } else {
            goto copying;
          }
        } else if(!destinationFileInfo.exists()) {
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

      if(!this->copyFile(sourcePath, destinationPath,
        sourceFileInfo.size())) {
        this->errorOccurred = true;
      }

      goto done;

      cloning:
      emit this->triggerCurrentOperation(tr("Creating directory"));

      if(root.mkdir(destinationPath) && this->transferFileAttributes(
        sourcePath, destinationPath)) {
        this->log->logEvent(tr("Successfully recreated directory '%1'"
          " as '%2'.").arg(sourcePath, destinationPath));
      } else {
        this->errorOccurred = true;
        this->log->logError(tr("Failed to recreate directory '%1'"
          " as '%2'!").arg(sourcePath, destinationPath));
      }

      goto done;

      skipping:
      this->log->logEvent(tr("Skipping file or directory"
        " '%1'.").arg(sourcePath));
      emit this->triggerCurrentOperation(tr("Skipping item"));

      done:
      this->processedCount++;
      this->processedSize += sourceFileInfo.size();
    }
  }

  /*
   * Remove obsolete files in the backup destination folder if the user
   * did not requested otherwise
   */
  if(this->progress) {
    if(!this->keepObsolete) {
      this->triggerOverallProgress(-1);
      QDirIterator i(this->target, WORKER_ITEM_FILTERS,
        QDirIterator::Subdirectories);
      root = QDir(this->target);
      emit triggerCurrentOperation(tr("Searching for obsolete item(s)"));
      emit triggerCurrentItem(tr("Please, wait..."));

      while(i.hasNext()) {
        QString currentFile(i.next());
        QFile correspondingFile(this->source + "/"
          + root.relativeFilePath(currentFile));
        QFileInfo currentFileInfo(currentFile);

        if(!correspondingFile.exists() && currentFileInfo.exists()) {
          emit this->triggerCurrentItem(currentFile);
          if(currentFileInfo.isDir()) {
            emit this->triggerCurrentOperation(tr("Removing folder"));

            QDir temp(currentFile);

            if(temp.removeRecursively()) {
              this->log->logEvent(tr("Successfully removed obsolete"
                " folder '%1'.").arg(currentFile));
            } else {
              this->errorOccurred = true;
              this->log->logError(tr("Failed to remove obsolete folder"
                " '%1'!").arg(currentFile));
            }
          } else {
            emit this->triggerCurrentOperation(tr("Removing file"));

            if(QFile::remove(currentFile)) {
              this->log->logEvent(tr("Successfully removed obsolete"
                " file '%1'.").arg(currentFile));
            } else {
              this->errorOccurred = true;
              this->log->logError(tr("Failed to remove obsolete"
                " file '%1'.").arg(currentFile));
            }
          }
        }
      }

      this->triggerOverallProgress(0);
    }
  }

  emit this->finished((int) ACTION_BACKUP);
}

/*!
 * \fn void Worker::analyze()
 * \brief Performs backup source location analysis.
 */
void Worker::analyze() {
  this->progress = true;
  emit this->started();

  this->reinitializeCounters();

  QDirIterator i(this->source, WORKER_ITEM_FILTERS,
    QDirIterator::Subdirectories);

  while(i.hasNext() && this->progress) {
    QString currentPath(i.next());
    QFileInfo current(currentPath);
    bool isSystem = false;

#if defined Q_OS_WIN
    DWORD attributes = GetFileAttributesW(
      currentPath.toStdWString().c_str());

    if(attributes == INVALID_FILE_ATTRIBUTES) {
      this->log->logError(tr("Unable to get attributes of file or"
        " directory '%1'!").arg(currentPath));
    }

    isSystem = attributes & FILE_ATTRIBUTE_SYSTEM;
    isSystem = isSystem || currentPath.startsWith(this->source
      + "$RECYCLE.BIN");
#elif defined Q_OS_LINUX
    isSystem = currentPath.startsWith(".local/share/Trash");
#else
#error Unsupported operating system!
#endif

    if(!isSystem) {
      if(current.isDir()) {
        this->directoriesCount++;
      } else {
        this->filesCount++;
      }

      this->totalCount++;
      this->size += current.size();

      emit this->triggerAnalysisProgress(this->filesCount,
        this->directoriesCount, this->size);
    }
  }

  emit this->finished((int) ACTION_ANALYSIS);
}

/*!
 * \fn void Worker::doWork()
 * \brief Slot triggered by main window Interface telling Worker to
 *        begin backup process.
 */
void Worker::doWork() {
  this->work();
}
