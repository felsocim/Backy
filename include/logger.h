/*!
 * \headerfile logger.h
 * \title Logger
 * \brief The logger.h file contains declarations related to the Logger class.
 * \author Marek Felsoci
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
 * \brief The Logger class is used by the Worker objects to keep trace of their activity.
 */
class Logger {
  private:
    /*!
     * \variable Logger::eventFile
     * \brief Points to a QFile instance corresponding to the event log file.
     */
    QFile * eventFile,
    /*!
     * \variable Logger::errorFile
     * \brief Points to a QFile instance corresponding to the error log file.
     */
      * errorFile;
    /*!
     * \variable Logger::eventStream
     * \brief Points to a QTextStream instance holding the event log contents before being written to the corresponding log file.
     */
    QTextStream * eventStream,
    /*!
     * \variable Logger::errorStream
     * \brief Points to a QTextStream instance holding the error log contents before being written to the corresponding log file.
     */
      * errorStream;
    /*!
     * \variable Logger::accessible
     * \brief Boolean indicating whether the log files are ready to use or not.
     */
    bool accessible;

  public:
    Logger(const QString &path, const QString &eventFile, const QString &errorFile);
    ~Logger();
    void logEvent(const QString &eventMessage);
    void logError(const QString &errorMessage);
};

#endif // __LOGGER_H
