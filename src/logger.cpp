#include "../include/logger.h"

Logger::Logger(const QString &path, const QString &eventFile, const QString &errorFile) {
  this->eventFile = new QFile(path + "/" + eventFile);
  this->errorFile = new QFile(path + "/" + errorFile);

  QDir logs(path);

  if(!logs.exists()) {
    if(logs.mkdir(path)) {
      this->accessible = true;
    } else {
      this->accessible = false;
      qDebug() << "Warning: Unable to create log directory at path:" << path;
    }
  } else {
    this->accessible = true;
  }

  if(this->accessible) {
    if(this->eventFile->open(QIODevice::WriteOnly) && this->errorFile->open(QIODevice::WriteOnly)) {
      this->eventStream = new QTextStream(this->eventFile);
      this->errorStream = new QTextStream(this->errorFile);
      this->accessible = true;
    } else {
      this->accessible = false;
      qDebug() << "Warning: Unable to create/open log files (" << eventFile << ", " << errorFile << ")! No logs will be created for this instance!";
    }
  }
}

Logger::~Logger() {
  if(this->accessible) {
    this->eventStream->~QTextStream();
    this->errorStream->~QTextStream();
    this->eventFile->close();
    this->errorFile->close();
  }

  delete this->eventFile;
  delete this->errorFile;
}

void Logger::logEvent(const QString &eventMessage) {
  if(this->accessible)
    (*this->eventStream) << eventMessage << endl;
}

void Logger::logError(const QString &errorMessage) {
  if(this->accessible)
    (*this->errorStream) << errorMessage << endl;
}
