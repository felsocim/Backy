#include "../include/producer.h"

Producer::Producer() : Worker() {
  this->root = nullptr;
  this->filesCount = 0;
  this->directoriesCount = 0;
  this->itembufferSize = DEFAULT_ITEM_BUFFER_SIZE;
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

void Producer::setRoot(const QString &root) {
  this->root = new QDir(root);
}

void Producer::setItemBufferSize(size_t itembufferSize) {
  this->itembufferSize = itembufferSize;
}

void Producer::reinitializeCounters() {
  this->directoriesCount = 0;
  this->filesCount = 0;
}

void Producer::createLogsAt(const QString &path) {
  this->log = new Logger(path, PRODUCER_EVENT_LOG_FILE_NAME, PRODUCER_ERROR_LOG_FILE_NAME);
}

void Producer::work() {
  emit this->started();

  QDirIterator i(this->root->absolutePath(), WORKER_ITEM_FILTERS, QDirIterator::Subdirectories);
  qint64 processedCount = 0;

  this->log->logEvent(tr("Producer process has begun working."));

  while(i.hasNext() && this->progress) {
    QFileInfo current = QFileInfo(i.next());

    this->lock->lock();

    while(this->buffer->size() == this->itembufferSize) {
      this->log->logEvent(tr("Producer process is waiting."));
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

    this->log->logEvent(tr("Producer process enqueued '%1' in the shared item buffer.").arg(current.fileName()));
    this->notFull->wakeOne();
    this->lock->unlock();
  }

  emit this->finished();

  this->log->logEvent(tr("Producer process has finished working."));
}

void Producer::analyze() {
  emit this->started();

  this->directoriesCount = 0;
  this->filesCount = 0;
  this->size = 0;

  QDirIterator i(this->root->absolutePath(), WORKER_ITEM_FILTERS, QDirIterator::Subdirectories);

  while(i.hasNext()) {
    QFileInfo current = QFileInfo(i.next());
    if(current.isDir()) {
      this->directoriesCount++;
    } else {
      this->filesCount++;
    }
    this->size += current.size();
    emit this->triggerAnalysisProgress(this->filesCount, this->directoriesCount, this->size);
  }

  this->log->logEvent(
    tr("During analysis of the source location '%1' the program has discovered ").arg(this->root->absolutePath()) +
    tr("%n folder(s), ", nullptr, this->directoriesCount) +
    tr("%n file(s) ", nullptr, this->filesCount) +
    tr("in %n byte(s).", nullptr, this->size)
  );

  emit this->finished();
}
