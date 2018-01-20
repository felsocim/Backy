#ifndef __PRODUCER_H
#define __PRODUCER_H

#include <QString>
#include <QThread>
#include <QDirIterator>
#include <QFileInfo>
#include <QMutex>
#include <QWaitCondition>
#include <queue>
#include "item.h"
#include "logger.h"

using namespace std;

class Producer : public QThread {
  private:
    queue<Item> * buffer;
    QMutex * lock;
    QWaitCondition * notEmpty,
      * notFull;
    QString root;
    int filesCount,
      directoriesCount;
    size_t bufferMax;
    qint64 size;
    Logger * log;
  public:
    Producer(
      queue<Item> * buffer,
      QMutex * lock,
      QWaitCondition * notEmpty,
      QWaitCondition * notFull,
      size_t bufferMax,
      QString eventLog,
      QString errorLog
    );
    ~Producer();
    QString getRoot();
    int getFilesCount();
    int getDirectoriesCount();
    size_t getSize();
    void setRoot(QString root);
    void run();
    void analyze();
};

#endif // __PRODUCER_H
