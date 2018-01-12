#include "reader.h"

Reader::Reader(){}

Reader::Reader(int id, QString begin, std::queue<Item> *buffer, int bufferLimit, QMutex *lock, QWaitCondition *conditionNotEmpty, QWaitCondition *conditionNotFull) {
  this->id = id;
  this->begin = begin;
  this->buffer = buffer;
  this->bufferLimit = bufferLimit;
  this->lock = lock;
  this->conditionNotEmpty = conditionNotEmpty;
  this->conditionNotFull = conditionNotFull;
}

void Reader::run() {
  QDirIterator list(this->begin, QDirIterator::Subdirectories);
  QString current;
  while(list.hasNext()) {
    this->lock->lock();
    while(this->buffer->size() == this->bufferLimit) {
      this->conditionNotFull->wait(this->lock);
    }
    current = list.next();
    QFileInfo item(current);
    this->buffer->push(Item(current, (item.isDir() ? ITEM_DIRECTORY : ITEM_FILE)));
    this->conditionNotEmpty->wakeAll();
    this->lock->unlock();
  }
}
