#ifndef __PREFERENCES_H
#define __PREFERENCES_H

#include <QDialog>
#include <QFileDialog>
#include <QStringList>
#include <QMessageBox>
#include "common.h"

#if defined Q_OS_LINUX
#include <QProcess>
#include <QRegExp>
#elif defined Q_OS_WIN
#include <Windows.h>
#else
#error "Unsupported operating system!"
#endif

namespace Ui {
  class Preferences;
}

class Preferences : public QDialog
{
  Q_OBJECT
  private:
    Ui::Preferences * ui;
    QFileDialog * browseLogsLocation;
    qint64 itemBufferSize,
      copyBufferSize;
    QString logsLocation;
    bool logsLocationChanged;
    void setCurrents();

  public:
    explicit Preferences(QWidget * parent = 0);
    ~Preferences();
    qint64 getItemBufferSize() const;
    qint64 getCopyBufferSize() const;
    QString getLogsLocation() const;
    void setItemBufferSize(qint64 itemBufferSize);
    void setCopyBufferSize(qint64 copyBufferSize);
    void setLogsLocation(QString logsLocation);
    void setDefaults();

  private slots:
    void onBrowseLogsLocation();
    void onChooseLogsLocation(QString selected);
    void onDiscard();
    void onSave();

  signals:
    void triggerSave();
};

#endif // __PREFERENCES_H
