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
  this->log = new Logger(eventLog, errorLog);
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

void Consumer::setTarget(QString target) {
  this->target = target;
}

void Consumer::setDetectedCount(int detectedCount) {
  this->detectedCount = 1 + detectedCount;
}

void Consumer::run() {
  this->log->logEvent("Consumer has started");
  do {
    this->lock->lock();
    while(this->buffer->empty()) {
      this->notFull->wait(this->lock);
    }
    cout << this->buffer->front() << endl;
    this->processedCount++;
    this->buffer->pop();
    this->notEmpty->wakeOne();
    this->lock->unlock();
  } while(this->processedCount < this->detectedCount);
  this->log->logEvent("Consumer has finished");
}
