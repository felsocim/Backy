/*!
 * This file is a part of Backy project, a simple backup creation and
 * maintaining solution.
 * 
 * Copyright (C) 2018 Marek Felsoci <marek.felsoci@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The complete license text can be found in the 'LICENSE' file in the root of
 * the application's repository.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * \file logger.h
 * \brief Contains declarations related to Logger class.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0.1
 */
#ifndef __LOGGER_H
#define __LOGGER_H

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include "common.h"

/*!
 * \class Logger
 * \brief Provides Worker objects with the possibility to keep trace of
 *        their activity.
 */
class Logger {
  private:
    /*!
     * \var Logger::eventFile
     * \brief Points to a QFile instance handling the event log file.
     */
    QFile * eventFile,
    /*!
     * \var Logger::errorFile
     * \brief Points to a QFile instance handling the error log file.
     */
      * errorFile;
    /*!
     * \var Logger::eventStream
     * \brief Points to a QTextStream instance which buffers event log
     *        contents before writing them to the event log file.
     */
    QTextStream * eventStream,
    /*!
     * \var Logger::errorStream
     * \brief Points to a QTextStream instance which buffers error log
     *        contents before writing them to the error log file.
     */
      * errorStream;
    /*!
     * \var Logger::accessible
     * \brief Indicates whether the log files are ready to use or not.
     */
    bool accessible;

  public:
    Logger(const QString &path, const QString &eventFile,
      const QString &errorFile);
    ~Logger();
    void logEvent(const QString &eventMessage);
    void logError(const QString &errorMessage);
};

#endif // __LOGGER_H
