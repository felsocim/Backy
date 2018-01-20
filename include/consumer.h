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
    int detectedCount,
      processedCount;
    size_t bufferMax;
    Logger * log;
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
    void setDetectedCount(int detectedCount);
    void run();
  signals:
    void currentStatus(int current, int overall);
};

#endif // __CONSUMER_H
