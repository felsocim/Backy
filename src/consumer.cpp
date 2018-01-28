#include "../include/consumer.h"

Consumer::Consumer() : Worker() {
  this->synchronize = DEFAULT_SYNCHRONIZE;
  this->keepObsolete = DEFAULT_KEEP_OBSOLETE;
  this->criterion = DEFAULT_COMPARISON_CRITERION;
  this->detectedCount = 0;
  this->detectedSize = 0;
  this->processedCount = 0;
  this->processedSize = 0;
  this->copyBufferSize = MEGABYTE * DEFAULT_COPY_BUFFER_SIZE;
  this->currentItem = nullptr;
  this->currentFile = nullptr;
  this->currentDirectory = nullptr;
}

bool Consumer::copyFile(QFile * source, QFile * destination, qint64 size) {
  char * bytes = new char[this->copyBufferSize];
  qint64 actuallyWritten = 0, toBeWritten = 0, code = 0, totalWritten = 0;

  if(source->open(QIODevice::ReadOnly))
    this->log->logEvent("Opened source file: "+ source->fileName());
  else
    this->log->logError("Unable to open source file: " + source->fileName());

  if(destination->open(QIODevice::WriteOnly))
    this->log->logEvent("Opened destination file: "+ destination->fileName());
  else
    this->log->logError("Unable to open destination file: " + destination->fileName());

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
      emit this->triggerOverallProgress((int) ceil(this->processedSize * 100 / this->detectedSize));

      memset(bytes, 0, this->copyBufferSize);
    }

    delete[] bytes;

    source->close();
    destination->close();

    if(!(code = destination->setPermissions(source->permissions()))) {
      this->log->logError("File copy failed (could not transfer file permissions)");
      return false;
    }

    QFileInfo s(*source);
    struct utimbuf time;

    time.actime = s.lastRead().toSecsSinceEpoch();
    time.modtime = s.lastModified().toSecsSinceEpoch();

    if((code = utime(destination->fileName().toStdString().c_str(), &time)) < 0){
      this->log->logError("File copy failed (could not transfer file dates)");
      return false;
    }

    return true;
  }

  error:
  delete[] bytes;
  this->log->logError("File copy failed (error code: " + QString::number(code) + ")");

  return false;
}

qint64 Consumer::getProcessedCount() const {
  return this->processedCount;
}

bool Consumer::didErrorOccurred() const {
  return this->errorOccurred;
}

void Consumer::setSource(QString source) {
  this->source = source;
}

void Consumer::setTarget(QString target) {
  this->target = target;
}

void Consumer::setSynchronize(bool synchronize) {
  this->synchronize = synchronize;
}

void Consumer::setKeepObsolete(bool keepObsolete) {
  this->keepObsolete = keepObsolete;
}

void Consumer::setCriterion(Criterion criterion) {
  this->criterion = criterion;
}

void Consumer::setDetectedCount(qint64 detectedCount) {
  this->detectedCount = detectedCount;
}

void Consumer::setDetectedSize(qint64 detectedSize) {
  this->detectedSize = detectedSize;
}

void Consumer::setCopyBufferSize(qint64 copyBufferSize) {
  this->copyBufferSize = MEGABYTE * copyBufferSize;
}

void Consumer::createLogsAt(const QString &path) {
  this->log = new Logger(path, CONSUMER_EVENT_LOG_FILE_NAME, CONSUMER_ERROR_LOG_FILE_NAME);
}

void Consumer::work() {
  emit this->started();

  this->processedCount = 0;
  this->processedSize = 0;
  this->errorOccurred = false;

  this->log->logEvent("Consumer has started");

  do {
    this->lock->lock();

    while(this->buffer->empty()) {
      this->notFull->wait(this->lock);
    }

    Item item = this->buffer->front();
    this->currentItem = new Item(item.getType(), item.getName(), item.getPath(), item.getLastModified(), item.getSize());

    emit this->triggerCurrentItem(this->currentItem->getPath());

    QString left(this->source + "/" + this->currentItem->getPath());
    QString existing(this->target + "/" + this->currentItem->getPath());
    QFile * before;
    QDir * beforeDirectory;

    if(this->currentItem->getType() == TYPE_FILE) {
      before = new QFile(left);
      this->currentFile = new QFile(existing);
    } else {
      beforeDirectory = new QDir(left);
      this->currentDirectory = new QDir(existing);
    }

    if(this->synchronize) {
      if(this->currentItem->getType() == TYPE_FILE) {
        if(this->currentFile->exists()) {
          if(this->currentItem->isSuperiorThan(existing, this->criterion)) {
            emit this->triggerCurrentOperation(tr("Removing previous version of file"));
            if(this->currentFile->remove())
              this->log->logEvent("Successfully removed previous version of file: " + existing);
            else {
              this->errorOccurred = true;
              this->log->logError("Failed to remove previous version of file: " + existing);
            }
            goto copying;
          } else {
            this->log->logEvent("Skipping item: " + this->currentItem->getName());
            emit this->triggerCurrentOperation(tr("Skipping item"));
            goto done;
          }
        } else {
          goto copying;
        }
      } else if(!this->currentDirectory->exists()) {
        goto cloning;
      } else {
        this->log->logEvent("Skipping item: " + this->currentItem->getName());
        emit this->triggerCurrentOperation(tr("Skipping item"));
        goto done;
      }
    } else {
      if(this->currentItem->getType() == TYPE_FILE) {
        goto copying;
      } else if(this->currentItem->getType() == TYPE_DIRECTORY) {
        goto cloning;
      } else {
        this->log->logEvent("Skipping item: " + this->currentItem->getName());
        emit this->triggerCurrentOperation(tr("Skipping item"));
        goto done;
      }
    }

    copying:
    emit this->triggerCurrentOperation(tr("Copying file"));

    if(this->copyFile(before, this->currentFile, this->currentItem->getSize()))
      this->log->logEvent("Successfully copied file: " + left + " to " + existing);
    else {
      this->errorOccurred = true;
      this->log->logError("Failed to copy file: " + left + " to " + existing);
    }

    goto done;

    cloning:
    emit this->triggerCurrentOperation(tr("Creating directory"));
    if(beforeDirectory->mkdir(existing))
      this->log->logEvent("Successfully cloned directory: " + left + " to " + existing);
    else {
      this->errorOccurred = true;
      this->log->logError("Failed to clone directory: " + left + " to " + existing);
    }

    done:
    this->processedCount++;
    this->buffer->pop();

    if(this->currentItem->getType() == TYPE_FILE) {
      delete before;
      delete this->currentFile;
    } else {
      delete beforeDirectory;
      delete this->currentDirectory;
    }

    delete this->currentItem;

    this->notEmpty->wakeOne();
    this->lock->unlock();
  } while(this->processedCount < this->detectedCount && this->progress);

  if(!this->progress) goto finish;

  if(!this->keepObsolete) {
    QDirIterator i(
      this->target,
      QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot, QDirIterator::Subdirectories
    );
    QDir t(this->target);

    while(i.hasNext()) {
      QString current(i.next());
      QString corresponding(this->source + "/" + t.relativeFilePath(current));

      if(!QFile::exists(corresponding)) {
        if(QFile::remove(current))
          this->log->logEvent("Successfully removed obsolete file: " + current);
        else
          this->log->logError("Failed to remove obsolete file: " + current);
          emit this->triggerCurrentOperation(tr("Removing folder"));
            this->errorOccurred = true;
          emit this->triggerCurrentOperation(tr("Removing file"));
            this->errorOccurred = true;
      }
    }
  }

  finish:
  emit this->finished();
  this->log->logEvent("Consumer has finished");
}
