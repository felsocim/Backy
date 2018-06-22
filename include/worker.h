#ifndef __WORKER_H
#define __WORKER_H

#include <QObject>
#include <QDir>
#include <QDirIterator>
#include <QMutex>
#include <QWaitCondition>
#include <queue>

#if defined Q_OS_LINUX
#include <utime.h>
#elif defined Q_OS_WIN
#include <Windows.h>
#include <sys/utime.h>
#else
#error Unsupported operating system!
#endif

#include <sys/types.h>
#include <math.h>
#include "common.h"
#include "logger.h"

#define WORKER_ITEM_FILTERS (FILE_ATTRIBUTE_ARCHIVE | \\
                            FILE_ATTRIBUTE_COMPRESSED | \\
                            FILE_ATTRIBUTE_ENCRYPTED | \\
                            FILE_ATTRIBUTE_HIDDEN | \\
                            FILE_ATTRIBUTE_READONLY | \\
                            FILE_ATTRIBUTE_DIRECTORY)

#define ERROR_UNKNOWN_COMPARISON_CRITERION -2

typedef enum {
  TYPE_FILE,
  TYPE_DIRECTORY
} Type;

typedef enum {
  CRITERION_MORE_RECENT,
  CRITERION_BIGGER
} Criterion;

class Worker : public QObject
{
  Q_OBJECT
  private:
    QString * source,
            * target;
    quint64 filesCount,
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
#if defined Q_OS_WIN
    qint8 compareItems(WIN32_FIND_DATA first, WIN32_FIND_DATA second) const;
    bool transferFileAttributes(const LPCSTR sourceFileName, const LPCSTR destinationFileName);
#elif defined Q_OS_LINUX
    // TODO: For Linux
#else
#error Unsupported operating system!
#endif
    bool copyFile(const QString &sourceFileName, const QString &destinationFileName, quint64 size);

  public:
    /* Constructor */
    Worker();

    /* Destructor */
    ~Worker();

    /* Getters */
    quint64 getFilesCount() const;
    quint64 getDirectoriesCount() const;
    quint64 getSize() const;
    quint64 getProcessedCount() const;
    bool getErrorOccurred() const;
    bool getProgress() const;

    /* Setters */
    void setSource(const QString &source);
    void setTarget(const QString &target);
    void setSynchronize(bool synchronize);
    void setKeepObsolete(bool keepObsolete);
    void setCriterion(Criterion criterion);
    void setCopyBufferSize(quint64 copyBufferSize);
    void setProgress(bool progress);

    /* Other methods */
    void reinitializeCounters();
    void createLogsAt(const QString &path);
    void work();


  public slots:
    void analyze();
    void doWork();

  signals:
    void triggerEvent(QString message);
    void triggerError(QString message);
    void triggerAnalysisProgress(quint64 files, quint64 directories, quint64 size);
    void triggerCurrentOperation(QString operation);
    void triggerCurrentItem(QString item);
    void triggerCurrentProgress(int currentItem);
    void triggerOverallProgress(int overall);
    void started();
    void finished();

};

#endif // __WORKER_H
