#include "logger.h"

Logger::Logger(QString eventFile, QString errorFile) {
  this->eventFile = new QFile(eventFile);
  this->errorFile = new QFile(errorFile);

  if(this->eventFile->open(QIODevice::WriteOnly) && this->errorFile->open(QIODevice::WriteOnly)) {
    this->eventStream = new QTextStream(this->eventFile);
    this->errorStream = new QTextStream(this->errorFile);
  } else {
    qDebug() << "Warning: Unable to open provided log files (" << eventFile << ", " << errorFile << ")! No logs will be created for this instance!";
  }
}

Logger::~Logger() {
  this->eventStream->~QTextStream();
  this->errorStream->~QTextStream();
  this->eventFile->close();
  this->errorFile->close();
  delete this->eventFile;
  delete this->errorFile;
}

void Logger::logEvent(QString eventMessage) {
  (*this->eventStream) << eventMessage << endl;
}

void Logger::logError(QString errorMessage) {
  (*this->errorStream) << errorMessage << endl;
}
