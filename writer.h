#ifndef WRITER_H
#define WRITER_H

#include <QThread>
#include <queue>
#include <QMutex>
#include <QWaitCondition>
#include <QString>
#include <QDebug>
#include "item.h"

class Writer : public QThread {
  private:
    std::queue<Item> * buffer;
    QMutex * lock;
    QWaitCondition * conditionNotEmpty,
      * conditionNotFull;
    bool * ended;
    bool loop;
  public:
    Writer();
    Writer(
      std::queue<Item> * buffer,
      QMutex * lock,
      QWaitCondition * conditionNotEmpty,
      QWaitCondition * conditionNotFull,
      bool * ended
    );
    void run();
};

#endif // WRITER_H
