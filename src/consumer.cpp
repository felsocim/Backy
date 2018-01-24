#include "consumer.h"

Consumer::Consumer(
  queue<Item> * buffer,
  QMutex * lock,
  QWaitCondition * notEmpty,
  QWaitCondition * notFull,
  size_t bufferMax,
  QString eventLog,
  QString errorLog
) {
  this->synchronize = false;
  this->buffer = buffer;
  this->lock = lock;
  this->notEmpty = notEmpty;
  this->notFull = notFull;
  this->bufferMax = bufferMax;
  this->detectedCount = 1;
  this->processedCount = 0;
  this->copyBufferSize = 1024;
  this->log = new Logger(eventLog, errorLog);
  //QObject::connect(this->currentFile, SIGNAL(bytesWritten(qint64)), this, SLOT(inputOutputProgress(qint64)));
}

Consumer::~Consumer() {
  delete this->log;
}

bool Consumer::copyFile(QFile * source, QFile * destination, qint64 size) {
  char * bytes = new char[this->copyBufferSize];
  qint64 code = 0;

  if(source->open(QIODevice::ReadOnly))
    this->log->logEvent("Opened source file: "+ source->fileName());
  else
    this->log->logError("Unable to open source file: " + source->fileName());

  if(destination->open(QIODevice::WriteOnly))
    this->log->logEvent("Opened destination file: "+ destination->fileName());
  else
    this->log->logError("Unable to open destination file: " + destination->fileName());

  if(source->isReadable() && destination->isWritable()) {
    while((code = source->read(bytes, this->copyBufferSize)) != 0) {
      if(code < 0)
        goto error;
      if((code = destination->write(bytes, code)) < 1)
        goto error;
      memset(bytes, 0, this->copyBufferSize);
      emit this->currentProgress((int) (((float) code / (float) size) * 100.0));
    }
    delete bytes;
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
  delete bytes;
  this->log->logError("File copy failed (error code: " + QString::number(code) + ")");
  return false;
}

int Consumer::getDetectedCount() {
  return this->detectedCount;
}

int Consumer::getProcessedCount() {
  return this->processedCount;
}

void Consumer::setSynchronize(bool synchronize) {
  this->synchronize = synchronize;
}

void Consumer::setSource(QString source) {
  this->source = source;
}

void Consumer::setTarget(QString target) {
  this->target = target;
}

void Consumer::setDetectedCount(int detectedCount) {
  this->detectedCount = detectedCount;
}

void Consumer::setCriterion(Criterion criterion) {
  this->criterion = criterion;
}

void Consumer::setKeepObsolete(bool keepObsolete) {
  this->keepObsolete = keepObsolete;
}

void Consumer::run() {
  this->log->logEvent("Consumer has started");
  do {
    this->lock->lock();
    while(this->buffer->empty()) {
      this->notFull->wait(this->lock);
    }
    Item item = this->buffer->front();
    this->current = new Item(item.getType(), item.getName(), item.getPath(), item.getLastModified(), item.getSize());

    emit this->currentItem(this->current->getPath() + "/" + this->current->getName());

    QString left(this->source + "/" + this->current->getPath());
    QString existing(this->target + "/" + this->current->getPath());
    QFile * before;
    QDir * beforeDirectory;
    if(this->current->getType() == TYPE_FILE) {
      before = new QFile(left);
      this->currentFile = new QFile(existing);
    } else {
      beforeDirectory = new QDir(left);
      this->currentDirectory = new QDir(existing);
    }

    if(this->synchronize) {
      if(this->current->getType() == TYPE_FILE) {
        if(this->currentFile->exists() && this->current->isSuperiorThan(existing, this->criterion)) {
          if(this->currentFile->remove())
            this->log->logEvent("Successfully removed previous version of file: " + existing);
          else
            this->log->logError("Failed to remove previous version of file: " + existing);
        }
        goto copying;
      } else {
        if(!this->currentFile->exists())
          goto cloning;
      }
    } else {
      if(this->current->getType() == TYPE_FILE)
        goto copying;
      else if(this->current->getType() == TYPE_DIRECTORY)
        goto cloning;
      else
        this->log->logEvent("Skipping item: " + this->current->getName());
    }

    copying:
      if(this->copyFile(before, this->currentFile, this->current->getSize()))
        this->log->logEvent("Successfully copied file: " + left + " to " + existing);
      else
        this->log->logError("Failed to copy file: " + left + " to " + existing);
    goto done;
    cloning:
      if(beforeDirectory->mkdir(existing))
        this->log->logEvent("Successfully cloned directory: " + left + " to " + existing);
      else
        this->log->logError("Failed to clone directory: " + left + " to " + existing);
    done:
    this->processedCount++;
    this->buffer->pop();

    if(this->current->getType() == TYPE_FILE) {
      delete before;
      delete this->currentFile;
    } else {
      delete beforeDirectory;
      delete this->currentDirectory;
    }

    delete this->current;

    emit overallProgress((int) (((float) this->processedCount / (float) this->detectedCount) * 100.0));
    this->notEmpty->wakeOne();
    this->lock->unlock();
  } while(this->processedCount < this->detectedCount);

  if(!this->keepObsolete) {
    QDirIterator i(this->target, QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDir t(this->target);
    while(i.hasNext()) {
      QString current(i.next());
      QString corresponding(this->source + "/" + t.relativeFilePath(current));
      if(!QFile::exists(corresponding)) {
        if(QFile::remove(current))
          this->log->logEvent("Successfully removed obsolete file: " + current);
        else
          this->log->logError("Failed to remove obsolete file: " + current);
      }
    }
  }

  this->log->logEvent("Consumer has finished");
}
