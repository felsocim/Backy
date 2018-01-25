#ifndef __PRODUCER_H
#define __PRODUCER_H

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

class Producer : public QThread {
  private:
    std::queue<Item> * buffer;
    QMutex * lock;
    QWaitCondition * notEmpty,
      * notFull;
    QDir * root;
    qint64 filesCount,
      directoriesCount,
      size;
    size_t bufferMax;
    Logger * log;
  public:
    Producer();
    ~Producer();
    qint64 getFilesCount() const;
    qint64 getDirectoriesCount() const;
    qint64 getSize() const;
    void setBuffer(std::queue<Item> * buffer);
    void setLock(QMutex * lock);
    void setNotEmpty(QWaitCondition * notEmpty);
    void setNotFull(QWaitCondition * notFull);
    void setRoot(QString &root);
    void setBufferMax(size_t bufferMax);
    void setLogger(QString &events, QString &errors);
    void run();
    void analyze();
};

#endif // __PRODUCER_H
