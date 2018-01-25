#include "producer.h"

Producer::Producer() {
  this->buffer = nullptr;
  this->lock = nullptr;
  this->notEmpty = nullptr;
  this->notFull = nullptr;
  this->root = nullptr;
  this->filesCount = 0;
  this->directoriesCount = 0;
  this->bufferMax = DEFAULT_BUFFER_MAX;
  this->size = 0;
  this->log = nullptr;
}

Producer::~Producer() {
  delete this->root;
  delete this->log;
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

void Producer::setBuffer(std::queue<Item> * buffer) {
  this->buffer = buffer;
}

void Producer::setLock(QMutex * lock) {
  this->lock = lock;
}

void Producer::setNotEmpty(QWaitCondition * notEmpty) {
  this->notEmpty = notEmpty;
}

void Producer::setNotFull(QWaitCondition * notFull) {
  this->notFull = notFull;
}

void Producer::setRoot(QString &root) {
  this->root = new QDir(root);
}

void Producer::setBufferMax(size_t bufferMax) {
  this->bufferMax = bufferMax;
}

void Producer::setLogger(QString &events, QString &errors) {
  this->log = new Logger(events, errors);
}

void Producer::run() {
  QDirIterator i(this->root->absolutePath(), QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
  this->log->logEvent("Producer has started");
  while(i.hasNext()) {
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
  this->log->logEvent("Producer has finished");
}

void Producer::analyze() {
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
}
