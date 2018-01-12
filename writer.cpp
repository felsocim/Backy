#include "writer.h"

Writer::Writer() {}

Writer::Writer(std::queue<Item> *buffer, QMutex *lock, QWaitCondition *conditionNotEmpty, QWaitCondition *conditionNotFull, bool * ended) {
  this->buffer = buffer;
  this->lock = lock;
  this->conditionNotEmpty = conditionNotEmpty;
  this->conditionNotFull = conditionNotFull;
  this->ended = ended;
  this->loop = true;
}

void Writer::run() {
  while(this->loop) {
    this->lock->lock();
    while(this->buffer->empty()) {
      qDebug() << "Writer waits";
      this->conditionNotEmpty->wait(this->lock);
    }
    qDebug() << this->buffer->front().getName().toUtf8();
    this->buffer->pop();
    if(*this->ended) {
      qDebug() << "Writer ended";
      this->loop = false;
    }
    this->conditionNotFull->wakeAll();
    this->lock->unlock();
  }
}
