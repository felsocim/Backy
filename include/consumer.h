#ifndef __CONSUMER_H
#define __CONSUMER_H

#include <iostream>
#include <sys/types.h>
#include <utime.h>
#include "worker.h"

class Consumer : public Worker {
  Q_OBJECT
  private:
    QString source,
      target;
    bool synchronize,
      keepObsolete;
    Criterion criterion;
    qint64 detectedCount,
      processedCount,
      bufferMax,
      copyBufferSize;
    Item * current;
    QFile * currentFile;
    QDir * currentDirectory;
    bool copyFile(QFile * source, QFile * destination, qint64 size);
  public:
    Consumer();
    ~Consumer();
    qint64 getProcessedCount() const;
    void setSource(QString source);
    void setTarget(QString target);
    void setSynchronize(bool synchronize);
    void setKeepObsolete(bool keepObsolete);
    void setCriterion(Criterion criterion);
    void setDetectedCount(int detectedCount);
    void setLogger(QString path);
    void run();
  signals:
    void currentItem(QString item);
    void currentProgress(int current);
    void overallProgress(int overall);
};

#endif // __CONSUMER_H
