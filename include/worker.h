#ifndef WORKER_H
#define WORKER_H

#include <QString>
#include <QThread>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QMutex>
#include <QWaitCondition>
#include <queue>
#include "item.h"
#include "logger.h"

class Worker : public QThread
{
  protected:
    std::queue<Item> * buffer;
    QMutex * lock;
    QWaitCondition * notEmpty,
      * notFull;
    Logger * log;
  public:
    Worker();
    void setBuffer(std::queue<Item> * buffer);
    void setLock(QMutex * lock);
    void setNotEmpty(QWaitCondition * notEmpty);
    void setNotFull(QWaitCondition * notFull);
    virtual void setLogger(QString path) = 0;
};

#endif // WORKER_H
