#ifndef __COMMON_H
#define __COMMON_H

#include <QString>
#include <QDebug>

typedef enum {
  TYPE_FILE,
  TYPE_DIRECTORY
} Type;

typedef enum {
  CRITERION_MORE_RECENT,
  CRITERION_BIGGER
} Criterion;

#if defined Q_OS_LINUX
#define DEFAULT_LOGS_LOCATION QDir::homePath() + "/.Backy"
#else
#error "Unsupported operating system!"
#endif

#define KILOBYTE (qint64) 1024
#define MEGABYTE (KILOBYTE * 1024)
#define GIGABYTE (MEGABYTE * 1024)
#define TERABYTE (GIGABYTE * 1024)

#define PRODUCER_EVENT_LOG_FILE_NAME "produder_event.log"
#define PRODUCER_ERROR_LOG_FILE_NAME "producer_error.log"
#define CONSUMER_EVENT_LOG_FILE_NAME "consumer_event.log"
#define CONSUMER_ERROR_LOG_FILE_NAME "consumer_error.log"
#define MAIN_EVENT_LOG_FILE_NAME "main_event.log"
#define MAIN_ERROR_LOG_FILE_NAME "main_error.log"
#define DEFAULT_COPY_BUFFER_SIZE 1
#define DEFAULT_ITEM_BUFFER_SIZE 5
#define DEFAULT_SYNCHRONIZE false
#define DEFAULT_KEEP_OBSOLETE false
#define DEFAULT_COMPARISON_CRITERION CRITERION_MORE_RECENT

#endif // __COMMON_H
