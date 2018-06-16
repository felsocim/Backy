#ifndef __WORKER_H
#define __WORKER_H

#include <QObject>
#include <QDir>
#include <QDirIterator>
#include <QMutex>
#include <QWaitCondition>
#include <queue>
#include "common.h"
#include "item.h"
#include "logger.h"

#define WORKER_ITEM_FILTERS (QDir::AllEntries | QDir::NoDotAndDotDot)

class Worker : public QObject
{
  Q_OBJECT
  protected:
    std::queue<Item> * buffer;
    QMutex * lock;
    QWaitCondition * notEmpty,
      * notFull;
    Logger * log;
    volatile bool progress;

  public:
    Worker();
    ~Worker();
    bool getProgress() const;
    void setBuffer(std::queue<Item> * buffer);
    void setLock(QMutex * lock);
    void setNotEmpty(QWaitCondition * notEmpty);
    void setNotFull(QWaitCondition * notFull);
    virtual void createLogsAt(const QString &path) = 0;
    virtual void work() = 0;
    void setProgress(bool progress);

  public slots:
    void doWork();

  signals:
    void started();
    void finished();
};

#endif // __WORKER_H
