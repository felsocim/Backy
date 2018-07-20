/*!
 * This file is a part of Backy project, a simple backup creation and
 * maintaining solution.
 * 
 * Copyright (C) 2018 Marek Felsoci <marek.felsoci@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The complete license text can be found in the 'LICENSE' file in the root of
 * the application's repository.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * \file worker.h
 * \brief Contains declarations and definitions related to Worker class.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0.1
 */
#ifndef __WORKER_H
#define __WORKER_H

#include <QObject>
#include <QDir>
#include <QDirIterator>
#include <QDateTime>
#include <QElapsedTimer>

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
 * \def WORKER_ITEM_FILTERS
 * \brief File type filters applied when iterating over the contents of
 *        backup source location.
 */
#define WORKER_ITEM_FILTERS (QDir::AllEntries | QDir::Hidden | \
                            QDir::NoDotAndDotDot)

/*!
 * \enum Action
 * \brief Enumerates types of actions that can be performed by Worker.
 */
typedef enum {
  ACTION_ANALYSIS,
  ACTION_BACKUP
} Action;

/*!
 * \enum Criterion
 * \relates Worker
 * \brief Enumerates comparison criterions.
 * 
 * These are used to decide whether whether a file in selected backup
 * source location is superior to its version potentially present in
 * selected target location.
 */
typedef enum {
  CRITERION_MORE_RECENT,
  CRITERION_BIGGER
} Criterion;

/*!
 * \class Worker
 * \brief Provides backup process-related functionalities.
 */
class Worker : public QObject
{
  Q_OBJECT
  private:
    /*!
     * \var Worker::source
     * \brief Path to backup source location.
     */
    QString source,
    /*!
     * \var Worker::target
     * \brief Path to backup target location.
     */
            target;
    /*!
     * \var Worker::filesCount
     * \brief Number of files detected during analysis of backup source
     *        location.
     */
    qint64 filesCount,
    /*!
     * \var Worker::directoriesCount
     * \brief Number of directories detected during analysis of backup
     *        source location.
     */
            directoriesCount,
    /*!
     * \var Worker::totalCount
     * \brief Sum of Worker::filesCount and Worker::directoriesCount.
     */
            totalCount,
    /*!
     * \var Worker::size
     * \brief Total size of items (files and directories) detected
     *        during analysis of backup source location.
     */
            size,
    /*!
     * \var Worker::processedCount
     * \brief Number of items (files and directories) in backup source
     *        location that have already been processed.
     */
            processedCount,
    /*!
     * \var Worker::processedSize
     * \brief Total size of items (files and directories) in backup
     *        source location that have already been processed.
     */
            processedSize,
    /*!
     * \var Worker::copyBufferSize
     * \brief Size of copy buffer used by file copy function.
     */
            copyBufferSize;
    /*!
     * \var Worker::synchronize
     * \brief Decides whether backup source location should be only
     *        synchronized with a previously created backup in target
     *        location.
     */
    bool synchronize,
    /*!
     * \var Worker::keepObsolete
     * \brief Decides whether any file in backup target location which
     *        does not exist in source location anymore should be kept
     *        or deleted.
     */
         keepObsolete,
    /*!
     * \var Worker::errorOccurred
     * \brief Indicates whether an error occurred during backup process.
     */
         errorOccurred;
    /*!
     * \var Worker::criterion
     * \brief Comparison criterion whether a file in backup source
     *        location is superior to its version potentially present
     *        in target location.
     */
    Criterion criterion;
    /*!
     * \var Worker::log
     * \brief Logger instance allowing to keep trace of any action
     *        performed within backup process.
     */
    Logger * log;
    /*!
     * \var Worker::progress
     * \brief Indicates whether the action being currently performed by
     *        Worker has to continue or to be aborted.
     */
    volatile bool progress;

  private:
    bool isCurrentSuperiorThanBackedUp(const QString &currentPath,
      const QString &backedUpPath) const;
    bool transferFileAttributes(const QString &sourcePath,
      const QString &destinationPath);
    bool copyFile(const QString &sourcePath,
      const QString &destinationPath, qint64 size);

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
     * \fn void triggerAnalysisProgress(qint64 files,
     *     qint64 directories, qint64 size)
     * \param files Current files count.
     * \param directories Current directories count.
     * \param size Total size of already processed items.
     * \brief Signal providing main window Interface with the
     *        information on how many \a files and \a directories of
     *        total \a size have already been discovered during backup
     *        source location analysis.
     */
    void triggerAnalysisProgress(qint64 files, qint64 directories,
      qint64 size);
    /*!
     * \fn void triggerCurrentOperation(QString operation)
     * \param operation Current operation's description.
     * \brief Signal telling main window Interface which \a operation
     *        is being performed by Worker.
     */
    void triggerCurrentOperation(QString operation);
    /*!
     * \fn void triggerCurrentItem(QString item)
     * \param item Currently processed item.
     * \brief Signal telling main window Interface instance which
     *        \a item (file or directory) is currently being processed.
     */
    void triggerCurrentItem(QString item);
    /*!
     * \fn void triggerCurrentProgress(int currentItem)
     * \param currentItem Current item progress value in percent.
     * \brief Signal providing main window Interface with the
     *        currently processed item (file or directory) progress
     *        value.
     */
    void triggerCurrentProgress(int currentItem);
    /*!
     * \fn void triggerOverallProgress(int overall)
     * \param overall Overall progress value in percent.
     * \brief Signal providing main window Interface with \a overall
     *        progress value in percent.
     */
    void triggerOverallProgress(int overall);
    /*!
     * \fn void started()
     * \brief Signal telling main window Interface that an action has
     *        begun within Worker.
     */
    void started();
    /*!
     * \fn void finished(int action)
     * \param action Type of finished action performed within Worker.
     * \brief Signal telling main window Interface which type of
     *        \a action being performed within Worker object
     *        has finished.
     * \see Action
     */
    void finished(int action);
};

#endif // __WORKER_H
