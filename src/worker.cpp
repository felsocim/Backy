#include "../include/worker.h"

Worker::Worker() {
  this->source = nullptr;
  this->target = nullptr;
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

#if defined Q_OS_WIN
qint8 Worker::compareItems(WIN32_FIND_DATA first, WIN32_FIND_DATA second) const {
  if(this->criterion == CRITERION_MORE_RECENT) {
    ULONGLONG firstLastWriteTime = (ULONGLONG) first.ftLastWriteTime.dwHighDateTime << 32 | first.ftLastWriteTime.dwLowDateTime,
              secondLastWriteTime = (ULONGLONG) second.ftLastWriteTime.dwHighDateTime << 32 | second.ftLastWriteTime.dwLowDateTime;
    if(firstLastWriteTime > secondLastWriteTime) {
      return 1;
    } else if(firstLastWriteTime < secondLastWriteTime) {
      return -1;
    }

    return 0;
  } else if(this->criterion == CRITERION_BIGGER) {
    if((ULONGLONG) first.nFileSizeHigh * ((ULONGLONG) MAXDWORD + (ULONGLONG) 1) + (ULONGLONG) first.nFileSizeLow > (ULONGLONG) second.nFileSizeHigh * ((ULONGLONG) MAXDWORD + (ULONGLONG) 1) + (ULONGLONG) second.nFileSizeLow) {
      return 1;
    } else if((ULONGLONG) first.nFileSizeHigh * ((ULONGLONG) MAXDWORD + (ULONGLONG) 1) + (ULONGLONG) first.nFileSizeLow < (ULONGLONG) second.nFileSizeHigh * ((ULONGLONG) MAXDWORD + (ULONGLONG) 1) + (ULONGLONG) second.nFileSizeLow) {
      return -1;
    }

    return 0;
  }

  return ERROR_UNKNOWN_COMPARISON_CRITERION;
}

bool Worker::transferFileAttributes(const LPCSTR sourceFileName, const LPCSTR destinationFileName) {
  HANDLE source = CreateFileA(sourceFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL),
         destination = CreateFileA(destinationFileName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  bool succeeded = true;
  LPBY_HANDLE_FILE_INFORMATION attributes;

  if(!GetFileInformationByHandle(source, attributes)) {
    this->log->logError(tr("Failed to read attributes from the source file (Error code: 0x%1)!").arg(QString::number(GetLastError(), 16)));
    succeeded = false;
  } else if(!SetFileInformationByHandle(destination, FileBasicInfo, attributes, sizeof(attributes))) {
    this->log->logError(tr("Failed to read attributes from the source file (Error code: 0x%1)!").arg(QString::number(GetLastError(), 16)));
    succeeded = false;
  }

  CloseHandle(source);
  CloseHandle(destination);

  return succeeded;
}
#elif defined Q_OS_LINUX
// TODO: For Linux
#else
#error Unsupported operating system!
#endif

bool Worker::copyFile(const QString &sourceFileName, const QString &destinationFileName, quint64 size) {
  QFile * source = new QFile(sourceFileName),
        * destination = new QFile(destinationFileName);
  char * bytes = new char[this->copyBufferSize];
  quint64 actuallyWritten = 0, toBeWritten = 0, totalWritten = 0;

  if(source->open(QIODevice::ReadOnly))
    this->log->logEvent(tr("Successfully opened source file '%1' for copying.").arg(source->fileName()));
  else
    this->log->logError(tr("Failed to open source file '%1' for copying.").arg(source->fileName()));

  if(destination->open(QIODevice::WriteOnly))
    this->log->logEvent(tr("Successfully opened destination file '%1' for copying.").arg(destination->fileName()));
  else
    this->log->logError(tr("Failed to open destination file '%1' for copying (%2).").arg(destination->fileName()).arg(destination->errorString()));

  if(source->isReadable() && destination->isWritable()) {
    while((toBeWritten = source->read(bytes, this->copyBufferSize)) != 0) {
      if(toBeWritten < 0) {
        goto error;
      }

      if((actuallyWritten = destination->write(bytes, toBeWritten)) != toBeWritten) {
        goto error;
      }

      totalWritten += actuallyWritten;
      this->processedSize += actuallyWritten;

      emit this->triggerCurrentProgress((int) ceil(totalWritten * 100 / size));
      emit this->triggerOverallProgress((int) ceil(this->processedSize * 100 / this->size));

      memset(bytes, 0, this->copyBufferSize);
    }

    delete[] bytes;

    source->close();
    destination->close();

    if(this->transferFileAttributes(source->fileName().toLocal8Bit().toStdString().c_str(), destination->fileName().toLocal8Bit().toStdString().c_str())) {
      this->log->logEvent(tr("Successfully transfered source file attributes to the destination file (%1)").arg(destination->fileName()));
    } else {
      this->log->logError(tr("Failed to transfer source file attributes to the destination file (%1)").arg(destination->fileName()));
    }

    this->log->logEvent(tr("Successfully copied file '%1' to '%2'.").arg(destination->fileName()).arg(source->fileName()));
    return true;
  }

  error:
  delete[] bytes;
  this->log->logError(tr("Failed to copy file '%1' to '%2'.").arg(destination->fileName()).arg(source->fileName()));

  source->close();
  destination->close();

  return false;
}

Worker::~Worker() {
  delete this->source;
  delete this->target;
  delete this->log;
}

quint64 Worker::getFilesCount() const {
  return this->filesCount;
}

quint64 Worker::getDirectoriesCount() const {
  return this->directoriesCount;
}

quint64 Worker::getSize() const {
  return this->size;
}

quint64 Worker::getProcessedCount() const {
  return this->processedCount;
}

bool Worker::getErrorOccurred() const {
  return this->errorOccurred;
}

bool Worker::getProgress() const {
  return this->progress;
}

void Worker::setSource(const QString &source) {
  this->source = new QString(source);
}

void Worker::setTarget(const QString &target) {
  this->target = new QString(target);
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

void Worker::setCopyBufferSize(quint64 copyBufferSize) {
  this->copyBufferSize = copyBufferSize;
}

void Worker::setProgress(bool progress) {
  this->progress = progress;

  if(!this->progress) {
    emit this->triggerEvent(tr("Backup process cancellation has been requested."));
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

#if defined Q_OS_WIN
  HANDLE currentHandle;
  WIN32_FIND_DATA currentData;
  QString * source = new QString("\\\\?\\" + (*this->source) + "\\*"),
          * destination = new QString("\\\\?\\" + (*this->target) + "\\"),
          temporary = nullptr;
  LPCSTR sourceUtf8 = source->toUtf8().toStdString().c_str();
  DWORD error;

  if((currentHandle = FindFirstFileW(sourceUtf8, currentData)) != INVALID_HANDLE_VALUE) {
    do {
      if((currentData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) || (currentData.dwFileAttributes & WORKER_ITEM_FILTERS)) {
        WIN32_FIND_DATA existingData;
        temporary = destination + QString(currentData.cFileName);
        LPCSTR temporaryUtf8 = temporary.toUtf8().toStdString().c_str();
        HANDLE existingHandle = FindFirstFileW(temporaryUtf8, existingData);

        if(this->synchronize) {
          if(existingHandle == INVALID_HANDLE_VALUE) {
            error = GetLastError();
            if(error != ERROR_FILE_NOT_FOUND) {
              this->log->logError(tr("Unable to gather information about file '%1'! (Error code: 0x%2)").arg(temporary, QString::number(error, 16)));
              this->errorOccurred = true;
            } else {
              if(currentData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                goto cloning;
              } else {
                goto copying;
              }
            }
          } else {
            if(!(currentData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
              if(this->compareItems(currentData, existingData) == 1) {
                this->triggerCurrentOperation(tr("Removing older version of file"));

                if(!DeleteFileW(temporaryUtf8)) {
                  this->log->logError(tr("Failed to remove older version of file '%1'! (Error code 0x%2)").arg(temporary, QString::number(error, 16)));
                  this->errorOccurred = true;
                } else {
                  this->log->logEvent(tr("Successfully removed older version of file '%1'.").arg(temporary));
                }

                goto copying;
              }
            } else {
              goto skipping;
            }
          }
        } else {
          if(currentData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            goto cloning;
          } else {
            goto copying;
          }
        }

        copying:
        emit this->triggerCurrentOperation(tr("Copying file"));

        if(!this->copyFile(
             this->source + '/' + QString(currentData.cFileName),
             this->target + '/' + QString(currentData.cFileName),
             (quint64) ((ULONGLONG) currentData.nFileSizeHigh * ((ULONGLONG) MAXDWORD + (ULONGLONG) 1) + (ULONGLONG) currentData.nFileSizeLow))
           ) {
          this->errorOccurred = true;
        }

        goto done;

        cloning:
        emit this->triggerCurrentOperation(tr("Creating directory"));

        if(CreateDirectoryW(temporaryUtf8, NULL) && this->transferFileAttributes(sourceUtf8, temporaryUtf8)) {
          this->log->logEvent(tr("Successfully recreated directory '%1' as '%2'.").arg(source + QString(currentData.cFileName)).arg(temporary));
        } else {
          this->errorOccurred = true;
          this->log->logError(tr("Failed to recreate directory '%1' as '%2'.").arg(source + QString(currentData.cFileName)).arg(temporary));
        }

        goto done;

        skipping:
        this->log->logEvent(tr("Skipping item '%1'. The most recent version already exists in backup destination.").arg(QString(currentData.cFileName)));
        emit this->triggerCurrentOperation(tr("Skipping item"));

        done:
        FindClose(existingHandle);
      }
    } while(FindNextFileW(currentHandle, currentData) && this->progress);

    if(!this->progress) {
      error = GetLastError();
      if(error != ERROR_NO_MORE_FILES) {
        emit this->triggerError(tr("Browsing contents of the source location failed! (Error code: 0x%1)").arg(QString::number(error, 16)));
      }
    }
  } else {
    error = GetLastError();
    if(error != ERROR_FILE_NOT_FOUND) {
      emit this->triggerError(tr("Browsing contents of the source location failed! (Error code: 0x%1)").arg(QString::number(error, 16)));
    }
  }

  FindClose(currentHandle);

  if(!this->progress) {
    goto finish;
  }

  if(!this->keepObsolete) {
    LPCSTR targetUtf8 = target->toUtf8().toStdString().c_str();
    QString toBeDeleted;

    if((currentHandle = FindFirstFileW(targetUtf8, currentData)) != INVALID_HANDLE_VALUE) {
      do {
        if((currentData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) || (currentData.dwFileAttributes & WORKER_ITEM_FILTERS)) {
          temporary = source + QString(currentData.cFileName);
          toBeDeleted = destination + QString(currentData.cFileName);
          LPCSTR temporaryUtf8 = temporary.toUtf8().toStdString().c_str();

          if(FindFirstFileW(temporaryUtf8, existingData) == INVALID_HANDLE_VALUE) {
            error = GetLastError();
            if(error != ERROR_FILE_NOT_FOUND) {
              emit this->triggerError(tr("Source directory analysis failed! (Error code: 0x%1)").arg(QString::number(error, 16)));
            } else {
              emit this->triggerCurrentOperation(tr("Removing item"));

              if(DeleteFileW(toBeDeleted.toUtf8().toStdString().c_str())) {
                this->log->logEvent(tr("Successfully removed obsolete item '%1'.").arg(temporary));
              } else {
                this->errorOccurred = true;
                this->log->logError(tr("Failed to remove obsolete item '%1'.").arg(temporary));
              }
            }
          }
        }
      } while(FindNextFileW(currentHandle, currentData));

      error = GetLastError();
      if(error != ERROR_NO_MORE_FILES) {
        emit this->triggerError(tr("Source directory analysis failed! (Error code: 0x%1)").arg(QString::number(error, 16)));
      }
    } else {
      error = GetLastError();
      if(error != ERROR_FILE_NOT_FOUND) {
        emit this->triggerError(tr("Source directory analysis failed! (Error code: 0x%1)").arg(QString::number(error, 16)));
      }
    }
  }

  finish:
  FindClose(currentHandle);
  delete source;
  delete destination;
#elif defined Q_OS_LINUX
  // TODO: Linux version
#else
#error Unsupported operating system!
#endif

  emit this->finished();
}

void Worker::analyze() {
  emit this->started();

  this->reinitializeCounters();

#if defined Q_OS_WIN
  HANDLE currentHandle;
  WIN32_FIND_DATA currentData;
  QString * source = new QString("\\\\?\\" + this->source + "\\*");
  LPCSTR sourceUtf8 = source->toUtf8().toStdString().c_str();
  DWORD error;

  if((currentHandle = FindFirstFileW(sourceUtf8, currentData)) != INVALID_HANDLE_VALUE) {
    do {
      if((currentData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) || (currentData.dwFileAttributes & WORKER_ITEM_FILTERS)) {
        if(currentData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
          this->directoriesCount++;
        } else {
          this->filesCount++;
        }
        this->totalCount++;
        this->size += (quint64) ((ULONGLONG) currentData.nFileSizeHigh * ((ULONGLONG) MAXDWORD + (ULONGLONG) 1) + (ULONGLONG) currentData.nFileSizeLow);
        emit this->triggerAnalysisProgress(this->filesCount, this->directoriesCount, this->size);
      }
    } while(FindNextFileW(currentHandle, currentData));

    error = GetLastError();
    if(error != ERROR_NO_MORE_FILES) {
      emit this->triggerError(tr("Source directory analysis failed! (Error code: 0x%1)").arg(QString::number(error, 16)));
    }
  } else {
    error = GetLastError();
    if(error != ERROR_FILE_NOT_FOUND) {
      emit this->triggerError(tr("Source directory analysis failed! (Error code: 0x%1)").arg(QString::number(error, 16)));
    }
  }

  delete source;
#elif defined Q_OS_LINUX
  // TODO: Linux version
#else
#error Unsupported operating system!
#endif

  emit this->finished();
}

void Worker::doWork() {
  this->work();
}
