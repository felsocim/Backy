#ifndef WORKER_H
#define WORKER_H

#include <QString>
#include <QObject>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QMutex>
#include <QWaitCondition>
#include <queue>
#include "item.h"
#include "logger.h"

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
    virtual void createLogsAt(QString path) = 0;
    virtual void work() = 0;
    void setProgress(bool progress);
  public slots:
    void doWork();
  signals:
    void started();
    void finished();
};

#endif // WORKER_H
