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
 * \file interface.h
 * \brief Contains declarations related to Interface class.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0.1
 */
#ifndef __INTERFACE_H
#define __INTERFACE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStringList>
#include <QThread>
#include <QMessageBox>
#include <QSettings>
#include <QDesktopServices>
#include "worker.h"
#include "preferences.h"

/*!
* \namespace Ui
* \brief This namespace englobes Interface class.
*/
namespace Ui {
  class Interface;
}

/*!
 * \class Interface
 * \brief Provides graphical user interface for main application window
 *        and interconnects it with core backup functionalities.
 */
class Interface : public QMainWindow {
  Q_OBJECT
  public:
    explicit Interface(QWidget * parent = 0);
    ~Interface();

  private:
    /*!
     * \var Interface::ui
     * \brief Points to an instance of main window Interface object.
     */
    Ui::Interface * ui;
    /*!
     * \var Interface::preferences
     * \brief Points to an instance of Preferences dialog object.
     */
    Preferences * preferences;
    /*!
     * \var Interface::sourceDialog
     * \brief Points to an instance of QFileDialog providing backup
     *        source location selection dialog.
     */
    QFileDialog * sourceDialog,
    /*!
     * \var Interface::targetDialog
     * \brief Points to an instance of QFileDialog providing backup
     *        target location selection dialog.
     */
      * targetDialog;
    /*!
     * \var Interface::workerThread
     * \brief QThread instance which will be used to execute
     *        backup-related operations provided by a Worker object.
     */
    QThread workerThread;
    /*!
     * \var Interface::worker
     * \brief Points to an instance of Worker object providing
     *        backup-related functionalities.
     */
    Worker * worker;
    /*!
     * \var Interface::workerInProgress
     * \brief Boolean indicating whether Interface::worker is currently
     *        performing a backup-related operation.
     */
    bool workerInProgress,
    /*!
     * \var Interface::aborted
     * \brief Boolean indicating whether user has requested a backup
     *        operation cancellation.
     */
      aborted;

  private:
    bool inProgress();
    QStringList ready();
    void abort();
    void loadSettings();
    void saveSettings();
    void toggleControlsState(bool actionInProgress);

  private slots:
    void onBrowseSource();
    void onBrowseTarget();
    void onToggleSynchronize(bool checked);
    void onToggleKeepObsolete(bool checked);
    void onToggleCriterionMostRecent(bool checked);
    void onToggleCriterionBiggest(bool checked);
    void onEditPreferences();
    void onShowAboutBox();
    void onAbort();
    void onQuit();
    void onBeginBackup();
    void onChooseSource(QString selected);
    void onAnalysisProgress(qint64 files, qint64 directories,
      qint64 size);
    void onChooseTarget(QString selected);
    void onStatusCurrentOperation(QString operation);
    void onStatusCurrentItem(QString item);
    void onStatusCurrentProgress(int current);
    void onStatusOverallProgress(int overall);
    void onWorkerStarted();
    void onWorkerFinished(int action);
    void onSavePreferences();
    void onDocumentationSolicitation();

  signals:
    /*!
     * \fn void signalStart()
     * \brief Signal telling Interface#worker to begin backup process.
     */
    void signalStart();
    /*!
     * \fn void triggerAnalysis()
     * \brief Signal telling Interface#worker to perform bacup source
     *        location contents analysis.
     */
    void triggerAnalysis();
};

#endif // __INTERFACE_H
