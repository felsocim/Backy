#include "worker.h"

Worker::Worker() {
  this->buffer = nullptr;
  this->lock = nullptr;
  this->notEmpty = nullptr;
  this->notFull = nullptr;
  this->log = nullptr;
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
