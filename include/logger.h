#ifndef __LOGGER_H
#define __LOGGER_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "preferences.h"

class Logger {
  private:
    QFile * eventFile,
      * errorFile;
    QTextStream * eventStream,
      * errorStream;
    bool accessible;
  public:
    Logger(QString path, QString eventFile, QString errorFile);
    ~Logger();
    void logEvent(QString eventMessage);
    void logError(QString errorMessage);
};

#endif // __LOGGER_H
