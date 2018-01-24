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
  this->detectedCount = 1 + detectedCount;
}

void Consumer::setCriterion(Criterion criterion) {
  this->criterion = criterion;
}

void Consumer::setTemporary(QString temporary) {
  this->temporary = temporary;
}

void Consumer::setKeepObsolete(bool keepObsolete) {
  this->keepObsolete = keepObsolete;
}

void Consumer::inputOutputProgress(qint64 bytes) {
  emit this->currentProgress((int)((float) bytes / (float) this->current->getSize()), this->current->getName());
}

void Consumer::run() {
  this->log->logEvent("Consumer has started");
  QFile * temp;
  QTextStream * outstream, * instream;
  if(!this->keepObsolete) {
    temp = new QFile(this->temporary);
    if(temp->open(QIODevice::ReadWrite))
      this->log->logEvent("Successfully opened temporary file: " + this->temporary);
    else
      this->log->logEvent("Failed to open temporary file: " + this->temporary);
    outstream = new QTextStream(temp), instream = new QTextStream(temp);
  }
  do {
    this->lock->lock();
    while(this->buffer->empty()) {
      this->notFull->wait(this->lock);
    }
    Item item = this->buffer->front();
    this->current = new Item(item.getType(), item.getName(), item.getPath(), item.getLastModified(), item.getSize());
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
      if(!this->keepObsolete)
        (*outstream) << this->current->getPath() << endl;
    } else {
      if(this->current->getType() == TYPE_FILE)
        goto copying;
      else if(this->current->getType() == TYPE_DIRECTORY)
        goto cloning;
      else
        this->log->logEvent("Skipping item: " + this->current->getName());
    }

    copying:
      if(before->copy(existing))
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
    while(!instream->atEnd()) {
      QString line(instream->readLine()), fullpath(this->target + "/" + line);
      QFile file(fullpath);
      if(file.remove())
        this->log->logEvent("Successfully removed obsolete file: " + fullpath);
      else
        this->log->logError("Failed to remove obsolete file: " + fullpath);
    }
    temp->close();
    delete temp;
    delete outstream;
    delete instream;
  }

  this->log->logEvent("Consumer has finished");
}
