#include "producer.h"

Producer::Producer() : Worker() {
  this->root = nullptr;
  this->filesCount = 0;
  this->directoriesCount = 0;
  this->bufferMax = DEFAULT_ITEM_BUFFER_SIZE;
  this->size = 0;
}

Producer::~Producer() {
  delete this->root;
}

qint64 Producer::getFilesCount() const {
  return this->filesCount;
}

qint64 Producer::getDirectoriesCount() const {
  return this->directoriesCount;
}

qint64 Producer::getSize() const {
  return this->size;
}

void Producer::setRoot(QString &root) {
  this->root = new QDir(root);
}

void Producer::setBufferMax(size_t bufferMax) {
  this->bufferMax = bufferMax;
}

void Producer::createLogsAt(QString path) {
  this->log = new Logger(path, PRODUCER_EVENT_LOG_FILE_NAME, PRODUCER_ERROR_LOG_FILE_NAME);
}

void Producer::work() {
  emit this->started();
  QDirIterator i(this->root->absolutePath(), QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
  this->log->logEvent("Producer has started");
  while(i.hasNext() && this->progress) {
    QFileInfo current = QFileInfo(i.next());
    this->lock->lock();
    while(this->buffer->size() == this->bufferMax) {
      this->log->logEvent("Producer is waiting");
      this->notEmpty->wait(this->lock);
    }
    this->buffer->push(
      Item(
        current.isDir() ? TYPE_DIRECTORY : TYPE_FILE,
        current.fileName(),
        this->root->relativeFilePath(current.filePath()),
        current.lastModified(),
        current.size()
      )
    );
    this->log->logEvent("Producer enqueued '" + current.fileName() + "'");
    this->notFull->wakeOne();
    this->lock->unlock();
  }
  emit this->finished();
  this->log->logEvent("Producer has finished");
}

void Producer::analyze() {
  emit this->started();
  this->directoriesCount = 0;
  this->filesCount = 0;
  this->size = 0;
  QDirIterator i(this->root->absolutePath(), QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
  while(i.hasNext()) {
    QFileInfo current = QFileInfo(i.next());
    if(current.isDir()) {
      this->directoriesCount++;
    } else {
      this->filesCount++;
    }
    this->size += current.size();
  }
  this->log->logEvent(
    "Producer analyzed root directory '" + this->root->absolutePath() + "' (" +
    QString::number(this->directoriesCount) +
    " folder(s) and " +
    QString::number(this->filesCount) +
    " file(s) in " +
    QString::number(this->size) +
    " bytes)"
  );
  emit this->finished();
}
