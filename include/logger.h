#ifndef __LOGGER_H
#define __LOGGER_H

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include "common.h"

class Logger {
  private:
    QFile * eventFile,
      * errorFile;
    QTextStream * eventStream,
      * errorStream;
    bool accessible;

  public:
    Logger(const QString &path, const QString &eventFile, const QString &errorFile);
    ~Logger();
    void logEvent(const QString &eventMessage);
    void logError(const QString &errorMessage);
};

#endif // __LOGGER_H
