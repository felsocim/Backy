/*!
 * \headerfile logger.cpp
 * \title Logger
 * \brief The logger.cpp file contains definitions related to the Logger class.
 * \author Marek Felsoci
 */
#include "../include/logger.h"

/*!
 * \fn Logger::Logger(const QString &path, const QString &eventFile, const QString &errorFile)
 * \param path Path to the log files location.
 * \param eventFile Event log file name.
 * \param errorFile Error log file name.
 * \brief Constructor of the Logger class.
 */
Logger::Logger(const QString &path, const QString &eventFile, const QString &errorFile) {
  this->eventFile = new QFile(path + "/" + eventFile);
  this->errorFile = new QFile(path + "/" + errorFile);

  QDir logs(path);

  /* If the logs folder does not exists create it */
  if(!logs.exists()) {
    if(logs.mkpath(path)) {
      this->accessible = true;
    } else {
      this->accessible = false;
      qDebug() << QCoreApplication::translate("Logger", "Warning: Failed to create log directory in '%1'! No logs will be created during this instance.").arg(path);
    }
  } else {
    this->accessible = true;
  }

  /* Open log files */
  if(this->accessible) {
    if(this->eventFile->open(QIODevice::WriteOnly) && this->errorFile->open(QIODevice::WriteOnly)) {
      this->eventStream = new QTextStream(this->eventFile);
      this->errorStream = new QTextStream(this->errorFile);
      this->accessible = true;
    } else {
      this->accessible = false;
      qDebug() << QCoreApplication::translate("Logger", "Warning: Failed to create or open log files: '%1', '%2'! No logs will be created for this instance.").arg(eventFile).arg(errorFile);
    }
  }
}

/*!
 * \fn Logger::~Logger()
 * \brief Destructor of the Logger class.
 */
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

/*!
 * \fn void Logger::logEvent(const QString &eventMessage)
 * \param eventMessage Event message to be logged.
 * \brief Add new message to the event log.
 */
void Logger::logEvent(const QString &eventMessage) {
  if(this->accessible)
    (*this->eventStream) << eventMessage << endl;
}

/*!
 * \fn void Logger::logError(const QString &errorMessage)
 * \param errorMessage Error message to be logged.
 * \brief Add new message to the error log.
 */
void Logger::logError(const QString &errorMessage) {
  if(this->accessible)
    (*this->errorStream) << errorMessage << endl;
}
