#include "worker.h"

Worker::Worker() {
  this->buffer = nullptr;
  this->lock = nullptr;
  this->notEmpty = nullptr;
  this->notFull = nullptr;
  this->log = nullptr;
  this->progress = true;
}

Worker::~Worker() {
  delete this->log;
}

bool Worker::getProgress() const {
  return this->progress;
}

void Worker::setBuffer(std::queue<Item> * buffer) {
  this->buffer = buffer;
}

void Worker::setLock(QMutex * lock) {
  this->lock = lock;
}

void Worker::setNotEmpty(QWaitCondition * notEmpty) {
  this->notEmpty = notEmpty;
}

void Worker::setNotFull(QWaitCondition * notFull) {
  this->notFull = notFull;
}

void Worker::setProgress(bool progress) {
  this->progress = progress;

  if(!this->progress)
    this->log->logEvent("Abort requested");
}

void Worker::doWork() {
  this->work();
}
