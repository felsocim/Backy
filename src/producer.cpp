#include "producer.h"

Producer::Producer(
  queue<Item> * buffer,
  QMutex * lock,
  QWaitCondition * notEmpty,
  QWaitCondition * notFull,
  size_t bufferMax,
  QString eventLog,
  QString errorLog
) {
  this->buffer = buffer;
  this->lock = lock;
  this->notEmpty = notEmpty;
  this->notFull = notFull;
  this->bufferMax = bufferMax;
  this->filesCount = 0;
  this->directoriesCount = 0;
  this->size = 0;
  this->log = new Logger(eventLog, errorLog);
}

Producer::~Producer() {
  delete this->log;
}

int Producer::getFilesCount() {
  return this->filesCount;
}

int Producer::getDirectoriesCount() {
  return this->directoriesCount;
}

size_t Producer::getSize() {
  return this->size;
}

void Producer::setRoot(QString root) {
  this->root = QDir(root);
}

void Producer::run() {
  QDirIterator i(this->root, QDirIterator::Subdirectories);
  this->log->logEvent("Producer has started");
  while(i.hasNext()) {
    QFileInfo current = QFileInfo(i.next());
    if(current.fileName().compare(".") != 0 && current.fileName().compare("..") != 0) {
      this->lock->lock();
      while(this->buffer->size() == this->bufferMax) {
        this->log->logEvent("Producer is waiting");
        this->notEmpty->wait(this->lock);
      }
      this->buffer->push(
        Item(
          current.isDir() ? TYPE_DIRECTORY : TYPE_FILE,
          current.fileName(),
          current.filePath(),
          current.lastModified(),
          current.size()
        )
      );
      this->log->logEvent("Producer enqueued '" + current.fileName() + "'");
      this->notFull->wakeOne();
      this->lock->unlock();
    }
  }
  this->log->logEvent("Producer has finished");
}

void Producer::analyze() {
  QDirIterator i(this->root, QDirIterator::Subdirectories);
  while(i.hasNext()) {
    QFileInfo current = QFileInfo(i.next());
    if(current.fileName().compare(".") != 0 && current.fileName().compare("..") != 0) {

      if(current.isDir()) {
        this->directoriesCount++;
      } else {
        this->filesCount++;
      }
      this->size += current.size();
    }
  }
  this->log->logEvent(
    "Producer analyzed root directory '" + this->root + "' (" +
    QString::number(this->directoriesCount) +
    " folder(s) and " +
    QString::number(this->filesCount) +
    " file(s) in " +
    QString::number(this->size) +
    " bytes)"
  );
}
