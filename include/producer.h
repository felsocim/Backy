#ifndef __PRODUCER_H
#define __PRODUCER_H

#include "worker.h"

class Producer : public Worker {
  private:
    QDir * root;
    qint64 filesCount,
      directoriesCount,
      size;
    size_t bufferMax;
  public:
    Producer();
    ~Producer();
    qint64 getFilesCount() const;
    qint64 getDirectoriesCount() const;
    qint64 getSize() const;
    void setRoot(QString &root);
    void setBufferMax(size_t bufferMax);
    void run();
    void createLogsAt(QString path);
    void analyze();
};

#endif // __PRODUCER_H
