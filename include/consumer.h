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


class Consumer : public QThread {
  Q_OBJECT
  private:
    std::queue<Item> * buffer;
    QMutex * lock;
    QWaitCondition * notEmpty,
      * notFull;
    QString source,
      target;
    bool synchronize,
      keepObsolete;
    Criterion criterion;
    qint64 detectedCount,
      processedCount,
      bufferMax,
      copyBufferSize;
    Logger * log;
    Item * current;
    QFile * currentFile;
    QDir * currentDirectory;
    bool copyFile(QFile * source, QFile * destination, qint64 size);
  public:
    Consumer();
    ~Consumer();
    qint64 getProcessedCount() const;
    void setBuffer(std::queue<Item> * buffer);
    void setLock(QMutex * lock);
    void setNotEmpty(QWaitCondition * notEmpty);
    void setNotFull(QWaitCondition * notFull);
    void setSource(QString source);
    void setTarget(QString target);
    void setSynchronize(bool synchronize);
    void setKeepObsolete(bool keepObsolete);
    void setCriterion(Criterion criterion);
    void setDetectedCount(int detectedCount);
    void setLogger(QString &events, QString &errors);
    void run();
  signals:
    void currentItem(QString item);
    void currentProgress(int current);
    void overallProgress(int overall);
};

#endif // __CONSUMER_H
