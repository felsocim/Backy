#ifndef __WORKER_H
#define __WORKER_H

#include <QObject>
#include <QDir>
#include <QDirIterator>
#include <QDateTime>

#if defined Q_OS_LINUX
#include <utime.h>
#elif defined Q_OS_WIN
#include <Windows.h>
#else
#error Unsupported operating system!
#endif

#include <sys/types.h>
#include <math.h>
#include "common.h"
#include "logger.h"

#define WORKER_ITEM_FILTERS (QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot)

typedef enum {
  ACTION_ANALYSIS,
  ACTION_BACKUP
} Action;

typedef enum {
  CRITERION_MORE_RECENT,
  CRITERION_BIGGER
} Criterion;

class Worker : public QObject
{
  Q_OBJECT
  private:
    QString source,
            target;
    qint64 filesCount,
            directoriesCount,
            totalCount,
            size,
            processedCount,
            processedSize,
            copyBufferSize;
    bool synchronize,
         keepObsolete,
         errorOccurred;
    Criterion criterion;
    Logger * log;
    volatile bool progress;

  private:
    bool isCurrentSuperiorThanBackedUp(const QString &currentPath, const QString &backedUpPath) const;
    bool transferFileAttributes(const QString &sourcePath, const QString &destinationPath);
    bool copyFile(const QString &sourcePath, const QString &destinationPath, qint64 size);

  public:
    /* Constructor */
    Worker();

    /* Destructor */
    ~Worker();

    /* Getters */
    qint64 getFilesCount() const;
    qint64 getDirectoriesCount() const;
    qint64 getSize() const;
    qint64 getProcessedCount() const;
    bool getErrorOccurred() const;
    bool getProgress() const;

    /* Setters */
    void setSource(const QString &source);
    void setTarget(const QString &target);
    void setSynchronize(bool synchronize);
    void setKeepObsolete(bool keepObsolete);
    void setCriterion(Criterion criterion);
    void setCopyBufferSize(qint64 copyBufferSize);
    void setProgress(bool progress);

    /* Other methods */
    void reinitializeCounters();
    void createLogsAt(const QString &path);
    void work();


  public slots:
    void analyze();
    void doWork();

  signals:
    void triggerAnalysisProgress(qint64 files, qint64 directories, qint64 size);
    void triggerCurrentOperation(QString operation);
    void triggerCurrentItem(QString item);
    void triggerCurrentProgress(int currentItem);
    void triggerOverallProgress(int overall);
    void started();
    void finished(int action);
};

#endif // __WORKER_H
