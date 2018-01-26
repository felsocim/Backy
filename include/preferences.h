#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#if defined Q_OS_WIN
#define DEFAULT_LOG_LOCATION "C:/"
#elif defined Q_OS_UNIX
#define DEFAULT_LOG_LOCATION "/var/log/Backy"
#else
#error "Unsupported operating system!"
#endif

#define PRODUCER_EVENT_LOG_FILE_NAME "produder_event.log"
#define PRODUCER_ERROR_LOG_FILE_NAME "producer_error.log"
#define CONSUMER_EVENT_LOG_FILE_NAME "consumer_event.log"
#define CONSUMER_ERROR_LOG_FILE_NAME "consumer_error.log"
#define MAIN_EVENT_LOG_FILE_NAME "main_event.log"
#define MAIN_ERROR_LOG_FILE_NAME "main_error.log"
#define DEFAULT_COPY_BUFFER_SIZE 1024
#define DEFAULT_BUFFER_MAX 5

namespace Ui {
  class Preferences;
}

class Preferences : public QDialog
{
  Q_OBJECT
  public:
    explicit Preferences(QWidget * parent = 0);
    ~Preferences();
  private:
    Ui::Preferences * ui;
};

#endif // PREFERENCES_H
