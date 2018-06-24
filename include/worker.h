/*!
 * \headerfile worker.h
 * \title Worker
 * \brief The worker.h file contains declarations and definitions related to the Worker class.
 * \author Marek Felsoci
 */
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

/*!
 * \macro WORKER_ITEM_FILTERS
 * \relates Worker
 * \brief File type filters applied when iterating over the backup source folder contents.
 */
#define WORKER_ITEM_FILTERS (QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot)

/*!
 * \enum Action
 * \relates Worker
 * \brief Enumerates types of actions that can be performed by Worker.
 */
typedef enum {
  ACTION_ANALYSIS,
  ACTION_BACKUP
} Action;

/*!
 * \enum Criterion
 * \relates Worker
 * \brief Enumerates comparison criterions that can be used to decide whether a file in the source location is superior to its previously backed up version when performing backup synchronization.
 */
typedef enum {
  CRITERION_MORE_RECENT,
  CRITERION_BIGGER
} Criterion;

/*!
 * \class Worker
 * \brief The Worker class provides all backup process-related functionalities.
 */
class Worker : public QObject
{
  Q_OBJECT
  private:
    /*!
     * \variable Worker::source
     * \brief String corresponding to the path to the backup source folder.
     */
    QString source,
    /*!
     * \variable Worker::target
     * \brief String corresponding to the path to the backup destination folder.
     */
            target;
    /*!
     * \variable Worker::filesCount
     * \brief Holds the number of files detected during analysis of the backup source folder contents.
     */
    qint64 filesCount,
    /*!
     * \variable Worker::directoriesCount
     * \brief Holds the number of directories detected during analysis of the backup source folder contents.
     */
            directoriesCount,
    /*!
     * \variable Worker::totalCount
     * \brief Holds the sum of the Worker::filesCount and the Worker::directoriesCount values.
     */
            totalCount,
    /*!
     * \variable Worker::size
     * \brief Holds the total size of items (files and directories) detected during analysis of the backup source folder contents.
     */
            size,
    /*!
     * \variable Worker::processedCount
     * \brief Holds the number of items (files and directories) from the backup source folder that have already been processed in the backup process.
     */
            processedCount,
    /*!
     * \variable Worker::processedSize
     * \brief Holds the total size of items (files and directories) from the backup source folder that have already been processed in the backup process.
     */
            processedSize,
    /*!
     * \variable Worker::copyBufferSize
     * \brief Holds the size of the copy buffer used during backup-related file operations, i.e. file copy.
     */
            copyBufferSize;
    /*!
     * \variable Worker::synchronize
     * \brief Boolean value which decides whether the application should only synchronize with a previously created backup.
     */
    bool synchronize,
    /*!
     * \variable Worker::keepObsolete
     * \brief Boolean value which decides whether the application should keep obsolete files in the location of previously created backup after synchronization.
     */
         keepObsolete,
    /*!
     * \variable Worker::errorOccurred
     * \brief Boolean value which indicates whether an error occurred during the backup process.
     */
         errorOccurred;
    /*!
     * \variable Worker::criterion
     * \brief Comparison criterion which decides whether a file in the source location is superior to its previously backed up version when performing backup synchronization.
     */
    Criterion criterion;
    /*!
     * \variable Worker::log
     * \brief A Logger instance allowing to keep trace of any action performed within the backup process by the Worker class instance.
     */
    Logger * log;
    /*!
     * \variable Worker::progress
     * \brief Boolean value which indicates whether the current action performed by the Worker class instance has to continue or to be aborted.
     */
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
    /*!
     * \fn void triggerAnalysisProgress(qint64 files, qint64 directories, qint64 size)
     * \param files Number of file entries that have already been discovered during the backup source folder contents analysis.
     * \param directories Number of directory entries that have already been discovered during the backup source folder contents analysis.
     * \param size Total size of entries that have already been discovered during the backup source folder contents analysis.
     * \brief Signal providing the main window interface instance with the information about current status of the backup source folder contents analysis.
     */
    void triggerAnalysisProgress(qint64 files, qint64 directories, qint64 size);
    /*!
     * \fn void triggerCurrentOperation(QString operation)
     * \param operation Current operation description.
     * \brief Signal telling the main window interface instance which operation is being performed by the Worker instance.
     */
    void triggerCurrentOperation(QString operation);
    /*!
     * \fn void triggerCurrentItem(QString item)
     * \param item Current item path.
     * \brief Signal telling the main window interface instance which item (file or directory) is being processed by the backup process.
     */
    void triggerCurrentItem(QString item);
    /*!
     * \fn void triggerCurrentProgress(int currentItem)
     * \param currentItem Current item processing progress (value from \b 0 to \b 100 percent).
     * \brief Signal providing the main window interface instance with the information about the progress in processing of the current item (file or directory) by the backup process.
     */
    void triggerCurrentProgress(int currentItem);
    /*!
     * \fn void triggerOverallProgress(int overall)
     * \param overall Overall backup process progress (value from \b 0 to \b 100 percent).
     * \brief Signal providing the main window interface instance with the information about overall backup process progress.
     */
    void triggerOverallProgress(int overall);
    /*!
     * \fn void started()
     * \brief Signal telling the main window interface instance that an action has begun within the Worker instance.
     */
    void started();
    /*!
     * \fn void finished(int action)
     * \param action The type of action performed within the Worker instance which has finished.
     * \brief Signal telling the main window interface instance which type of action being performed within the Worker instance has finished.
     */
    void finished(int action);
};

#endif // __WORKER_H
