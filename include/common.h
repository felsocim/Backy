/*!
 * \headerfile common.h
 * \title Shared definitions
 * \brief The common.h header file provides various pre-processor macros used throughout the whole project and includes commonly used headers.
 * \author Marek Felsoci
 */
#ifndef __COMMON_H
#define __COMMON_H

#include <QString>
#include <QDebug>
#include <QtGlobal>
#include <QStandardPaths>

/*!
 * \macro DEFAULT_LOGS_LOCATION
 * \relates common.h
 *
 * The DEFAULT_LOGS_LOCATION macro holds the path to the default location where application's log files should be saved.
 */
#if defined Q_OS_LINUX
#define DEFAULT_LOGS_LOCATION QDir::homePath() + "/.backy"
#elif defined Q_OS_WIN
#define DEFAULT_LOGS_LOCATION QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
#else
#error "Unsupported operating system!"
#endif

/*!
 * \macro KILOBYTE
 * \macro MEGABYTE
 * \macro GIGABYTE
 * \macro TERABYTE
 * \relates common.h
 *
 * These macros hold the number of bytes corresponding respectively to one kilobyte, one megabyte, one gigabyte and one terabyte.
 */
#define KILOBYTE (qint64) 1024
#define MEGABYTE (KILOBYTE * 1024)
#define GIGABYTE (MEGABYTE * 1024)
#define TERABYTE (GIGABYTE * 1024)

/*!
 * \macro WORKER_EVENT_LOG_FILE_NAME
 * \relates common.h
 *
 * The WORKER_EVENT_LOG_FILE_NAME defines the application's event log file name.
 */
#define WORKER_EVENT_LOG_FILE_NAME "backup_events.log"

/*!
 * \macro WORKER_ERROR_LOG_FILE_NAME
 * \relates common.h
 *
 * The WORKER_ERROR_LOG_FILE_NAME defines the application's event log file name.
 */
#define WORKER_ERROR_LOG_FILE_NAME "backup_errors.log"

/*!
 * \macro DEFAULT_COPY_BUFFER_SIZE
 * \relates common.h
 *
 * The DEFAULT_COPY_BUFFER_SIZE defines default size (in megabytes) of copy buffer used for file copy operations during the backup process. Initial value is 1 megabyte.
 */
#define DEFAULT_COPY_BUFFER_SIZE 1

/*!
 * \macro DEFAULT_SYNCHRONIZE
 * \relates common.h
 *
 * The DEFAULT_SYNCHRONIZE defines default boolean value which decides whether the application should only synchronize with a previously created backup. Initial value is \tt false.
 */
#define DEFAULT_SYNCHRONIZE false

/*!
 * \macro DEFAULT_KEEP_OBSOLETE
 * \relates common.h
 *
 * The DEFAULT_KEEP_OBSOLETE defines default boolean value which decides whether the application should keep obsolete files in the location of previously created backup after synchronization. Initial value is \tt false.
 */
#define DEFAULT_KEEP_OBSOLETE false

/*!
 * \macro DEFAULT_COMPARISON_CRITERION
 * \relates common.h
 *
 * The DEFAULT_COMPARISON_CRITERION defines default comparison criterion used to decide whether a file in the source location is superior to its previously backed up version when performing backup synchronization. Initial value is \tt CRITERION_MORE_RECENT which means that the most recent version of the file will be backed up.
 * \sa Criterion
 */
#define DEFAULT_COMPARISON_CRITERION CRITERION_MORE_RECENT

/*!
 * \macro DEFAULT_LOCALE_CODE
 * \relates common.h
 *
 * The DEFAULT_LOCALE_CODE defines default code of the language to display the application's user interface in. Initial value is \b sk which corresponds to \e{Slovak (Slovakia)}.
 */
#define DEFAULT_LOCALE_CODE "sk"

/*!
 * \macro LOCALE_RELATIVE_PATH
 * \relates common.h
 *
 * The LOCALE_RELATIVE_PATH holds the path to the directory containing translation files for the application relatively to the location of application's executable.
 */
#define LOCALE_RELATIVE_PATH "lang"

#endif // __COMMON_H
