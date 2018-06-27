/*!
 * This file is a part of Backy project, a simple backup creation and
 * maintaining solution.
 * 
 * Copyright (C) 2018 Marek Felsoci <marek.felsoci@gmail.com> (Feldev)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * \file logger.cpp
 * \brief Contains definitions related to Logger class.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0
 */
#include "../include/logger.h"

/*!
 * \fn Logger::Logger(const QString &path, const QString &eventFile,
 *     const QString &errorFile)
 * \param path Path to log files location.
 * \param eventFile Event log file name.
 * \param errorFile Error log file name.
 * \brief Constructor of Logger class.
 */
Logger::Logger(const QString &path, const QString &eventFile,
  const QString &errorFile) {
  this->eventFile = new QFile(path + "/" + eventFile);
  this->errorFile = new QFile(path + "/" + errorFile);

  QDir logs(path);

  /* If the logs folder does not exists create it */
  if(!logs.exists()) {
    if(logs.mkpath(path)) {
      this->accessible = true;
    } else {
      this->accessible = false;
      qDebug() << QCoreApplication::translate("Logger", "Warning:"
        " Failed to create log directory in '%1'! No logs will be"
        " created during this instance.").arg(path);
    }
  } else {
    this->accessible = true;
  }

  /* Open log files */
  if(this->accessible) {
    if(this->eventFile->open(QIODevice::WriteOnly)
      && this->errorFile->open(QIODevice::WriteOnly)) {
      this->eventStream = new QTextStream(this->eventFile);
      this->errorStream = new QTextStream(this->errorFile);
      this->accessible = true;
    } else {
      this->accessible = false;
      qDebug() << QCoreApplication::translate("Logger", "Warning:"
        " Failed to create or open log files: '%1', '%2'! No logs will"
        " be created for this instance.").arg(eventFile).arg(errorFile);
    }
  }
}

/*!
 * \fn Logger::~Logger()
 * \brief Destructor of Logger class.
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
 * \param eventMessage Message to be added to the event log.
 * \brief Adds a new message to the event log.
 */
void Logger::logEvent(const QString &eventMessage) {
  if(this->accessible)
    (*this->eventStream) << eventMessage << endl;
}

/*!
 * \fn void Logger::logError(const QString &errorMessage)
 * \param errorMessage Message to be added to the error log.
 * \brief Adds a new message to the error log.
 */
void Logger::logError(const QString &errorMessage) {
  if(this->accessible)
    (*this->errorStream) << errorMessage << endl;
}
