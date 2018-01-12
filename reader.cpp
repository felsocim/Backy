#include "reader.h"

Reader::Reader(){}

Reader::Reader(QString begin, std::queue<Item> *buffer, int bufferLimit, QMutex *lock, QWaitCondition *conditionNotEmpty, QWaitCondition *conditionNotFull, bool * ended) {
  this->begin = begin;
  this->buffer = buffer;
  this->bufferLimit = bufferLimit;
  this->lock = lock;
  this->conditionNotEmpty = conditionNotEmpty;
  this->conditionNotFull = conditionNotFull;
  this->ended = ended;
}

void Reader::run() {
  QDirIterator list(this->begin, QDirIterator::Subdirectories);
  qDebug() << "Reader starts with " << list.path();
  QString current;
  while(list.hasNext()) {
    this->lock->lock();
    while(this->buffer->size() == this->bufferLimit) {
      qDebug() << "Reader waits";
      this->conditionNotFull->wait(this->lock);
    }

    current = list.next();
    qDebug() << current;
    QFileInfo item(current);
    this->buffer->push(Item(current, (item.isDir() ? ITEM_DIRECTORY : ITEM_FILE)));
    if(!list.hasNext()) *this->ended = true;
    this->conditionNotEmpty->wakeAll();
    this->lock->unlock();
  }
  qDebug() << "Reader finished";
}
