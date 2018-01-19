#include "filer.h"

Filer::Filer(QObject * parent) {
  this->directoriesToProcess = 0;
  this->filesToProcess = 0;
  this->alreadyProcessed = 0;
}

void Filer::computeStats() {
  QDirIterator i(this->root, QDirIterator::Subdirectories);

  while(i.hasNext()) {
    QFileInfo current(i.next());

    if(current.isDir()) {
      this->directoriesToProcess++;
    } else {
      this->filesToProcess++;
    }
  }
}

void Filer::run() {
  QDirIterator i(this->root, QDirIterator::Subdirectories);

  while(i.hasNext()) {
    QString previous = i.next();
    QString path = previous;
    QFileInfo current(path);
    path.replace(this->root, this->target);

    if(current.isDir()) {
      QDir copy(path);
      if(copy.mkpath(".")) {
        copy.mkdir(".");
      } else {
        qDebug() << "Failed to create directory: " << path;
      }
    } else {
      QFile file(previous);
      if(!file.copy(path)) {
        qDebug() << path << " already exists";
      }
    }

    this->alreadyProcessed++;
    emit this->currentStatus(this->alreadyProcessed, previous);
  }
}

int Filer::getDirectoriesToProcess() {
  return this->directoriesToProcess;
}

int Filer::getFilesToProcess() {
  return this->filesToProcess;
}

void Filer::setRoot(QString root) {
  this->root = root;
}

void Filer::setTarget(QString target) {
  this->target = target;
}
