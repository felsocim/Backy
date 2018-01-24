#ifndef __CONSUMER_H
#define __CONSUMER_H

#include <QString>
#include <iostream>
#include <QThread>
#include <QDirIterator>
#include <QFileInfo>
#include <QMutex>
#include <QWaitCondition>
#include <queue>
#include <sys/types.h>
#include <utime.h>
#include "item.h"
#include "logger.h"

using namespace std;

class Consumer : public QThread {
  Q_OBJECT
  private:
    queue<Item> * buffer;
    QMutex * lock;
    QWaitCondition * notEmpty,
      * notFull;
    QString source,
      target,
      temporary;
    bool synchronize,
      keepObsolete;
    Criterion criterion;
    int detectedCount,
      processedCount;
    size_t bufferMax,
      copyBufferSize;
    Logger * log;
    Item * current;
    QFile * currentFile;
    QDir * currentDirectory;
    void signalCurrent(int current);
    bool copyFile(QFile * source, QFile * destination, qint64 size);
  public:
    Consumer(
      queue<Item> * buffer,
      QMutex * lock,
      QWaitCondition * notEmpty,
      QWaitCondition * notFull,
      size_t bufferMax,
      QString eventLog,
      QString errorLog
    );
    ~Consumer();
    int getDetectedCount();
    int getProcessedCount();
    void setSynchronize(bool synchronize);
    void setSource(QString source);
    void setTarget(QString target);
    void setDetectedCount(int detectedCount);
    void setCriterion(Criterion criterion);
    void setTemporary(QString temporary);
    void setKeepObsolete(bool keepObsolete);
    void run();
  signals:
    void currentItem(QString item);
    void currentProgress(int current);
    void overallProgress(int overall);
};

#endif // __CONSUMER_H
