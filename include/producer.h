#ifndef __PRODUCER_H
#define __PRODUCER_H

#include "worker.h"

class Producer : public Worker {
  Q_OBJECT
  private:
    QDir * root;
    qint64 filesCount,
      directoriesCount,
      size;
    size_t itembufferSize;

  public:
    Producer();
    ~Producer();
    qint64 getFilesCount() const;
    qint64 getDirectoriesCount() const;
    qint64 getSize() const;
    void setRoot(const QString &root);
    void setItemBufferSize(size_t itembufferSize);
    void reinitializeCounters();
    void createLogsAt(const QString &path);
    void work();

  public slots:
    void analyze();

  signals:
    void triggerAnalysisProgress(qint64 files, qint64 directories, qint64 size);
};

#endif // __PRODUCER_H
