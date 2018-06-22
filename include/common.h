#ifndef __COMMON_H
#define __COMMON_H

#include <QString>
#include <QDebug>
#include <QtGlobal>
#include <QStandardPaths>

#if defined Q_OS_LINUX
#define DEFAULT_LOGS_LOCATION QDir::homePath() + "/.backy"
#elif defined Q_OS_WIN
#define DEFAULT_LOGS_LOCATION QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
#else
#error "Unsupported operating system!"
#endif

#define KILOBYTE (qint64) 1024
#define MEGABYTE (KILOBYTE * 1024)
#define GIGABYTE (MEGABYTE * 1024)
#define TERABYTE (GIGABYTE * 1024)

#define WORKER_EVENT_LOG_FILE_NAME "backup_events.log"
#define WORKER_ERROR_LOG_FILE_NAME "backup_errors.log"
#define MAIN_EVENT_LOG_FILE_NAME "main_events.log"
#define MAIN_ERROR_LOG_FILE_NAME "main_errors.log"
#define DEFAULT_COPY_BUFFER_SIZE 1
#define DEFAULT_SYNCHRONIZE false
#define DEFAULT_KEEP_OBSOLETE false
#define DEFAULT_COMPARISON_CRITERION CRITERION_MORE_RECENT
#define DEFAULT_LOCALE_CODE "sk"
#define LOCALE_RELATIVE_PATH "lang"

#endif // __COMMON_H
