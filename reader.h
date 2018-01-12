#ifndef READER_H
#define READER_H

#include <QThread>
#include <queue>
#include <QMutex>
#include <QWaitCondition>
#include <QString>
#include <QDirIterator>
#include <QFileInfo>
#include <QDebug>
#include "item.h"

class Reader : public QThread {
  private:
    QString begin;
    std::queue<Item> * buffer;
    bool * ended;
    int nbItemsProcessed,
      bufferUsage,
      bufferLimit;
    QMutex * lock;
    QWaitCondition * conditionNotEmpty,
      * conditionNotFull;
  public:
    Reader();
    Reader(
      QString begin,
      std::queue<Item> * buffer,
      int bufferLimit,
      QMutex * lock,
      QWaitCondition * conditionNotEmpty,
      QWaitCondition * conditionNotFull,
      bool * ended
    );
    void run();
};

#endif // READER_H
