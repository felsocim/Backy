/*!
 * This file is a part of Backy project, a simple backup creation and
 * maintaining solution.
 * 
 * Copyright (C) 2018 Marek Felsoci <marek.felsoci@gmail.com> (Feldev)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * \file preferences.h
 * \brief Contains declarations related to Preferences class.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0
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
#error Unsupported operating system!
#endif

namespace Ui {
  class Preferences;
}

/*!
 * \class Preferences
 * \brief Provides graphical user interface for preferences dialog.
 */
class Preferences : public QDialog
{
  Q_OBJECT
  private:
    /*!
     * \var Preferences::ui
     * \brief Points to an instance of Preferences interface object.
     */
    Ui::Preferences * ui;
    /*!
     * \var Preferences::browseLogsLocation
     * \brief Points to an instance of QFileDialog providing log files
     *        folder selection dialog.
     */
    QFileDialog * browseLogsLocation;
    /*!
     * \var Preferences::copyBufferSize
     * \brief Size of copy buffer used by file copy function.
     */
    qint64 copyBufferSize;
    /*!
     * \var Preferences::logsLocation
     * \brief Path to log files location.
     */
    QString logsLocation;
    /*!
     * \var Preferences::locale
     * \brief Code of the current display language.
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
     * \brief Signal telling main window Interface#ui object to save
     *        current preferences on the disk
     * 
     * Application's preferences are stored in the Registry database on
     * Windows(R) respectively in a configuration file on Linux.
     */
    void triggerSave();
};

#endif // __PREFERENCES_H
