/*!
 * \headerfile preferences.h
 * \title Preferences
 * \brief The preferences.h file contains declarations related to the Preferences class.
 */
#ifndef __PREFERENCES_H
#define __PREFERENCES_H

#include <QDialog>
#include <QFileDialog>
#include <QDirIterator>
#include <QLocale>
#include <QStringList>
#include <QCoreApplication>
#include <QIcon>
#include <QVariant>
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

/*!
 * \class Preferences
 * \brief The Preferences class represents the application's preferences dialog interface.
 */
class Preferences : public QDialog
{
  Q_OBJECT
  private:
    /*!
     * \variable Preferences::ui
     * \brief Points to an instance of the pereferences dialog interface object.
     */
    Ui::Preferences * ui;
    /*!
     * \variable Preferences::browseLogsLocation
     * \brief Points to an instance of QFileDialog providing log files destination folder selection dialog.
     */
    QFileDialog * browseLogsLocation;
    /*!
     * \variable Preferences::copyBufferSize
     * \brief Holds the size of the copy buffer used during backup-related file operations, i.e. file copy.
     */
    qint64 copyBufferSize;
    /*!
     * \variable Preferences::logsLocation
     * \brief Holds the path to the log files destination folder.
     */
    QString logsLocation;
    /*!
     * \variable Preferences::locale
     * \brief Holds the code of the current language used within the application.
     */
    QVariant locale;

  private:
    void setCurrents();

  public:
    explicit Preferences(QWidget * parent = 0);
    ~Preferences();
    qint64 getCopyBufferSize() const;
    QString getLogsLocation() const;
    QVariant getLocale() const;
    void setCopyBufferSize(qint64 copyBufferSize);
    void setLogsLocation(QString logsLocation);
    void setLocale(QVariant locale);
    void setDefaults();

  private slots:
    void onBrowseLogsLocation();
    void onChooseLogsLocation(QString selected);
    void onDiscard();
    void onSave();

  signals:
    /*!
     * \fn void triggerSave()
     * \brief Signal telling the main window interface object Interface::ui to save current preferences on the disk (to the Registry database on Windows or to a configuration file on Linux).
     */
    void triggerSave();
};

#endif // __PREFERENCES_H
