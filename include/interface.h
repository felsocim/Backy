/*!
 * \headerfile interface.h
 * \title Interface
 * \brief The interface.h file contains declarations related to the Interface class.
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

namespace Ui {
  class Interface;
}

/*!
 * \class Interface
 * \brief The Interface class provides graphical user interface for main window of the application and interconnects it with the core backup functionalities.
 */
class Interface : public QMainWindow {
  Q_OBJECT
  public:
    explicit Interface(QWidget * parent = 0);
    ~Interface();

  private:
    /*!
     * \variable Interface::ui
     * \brief Points to an instance of the main window interface object.
     */
    Ui::Interface * ui;
    /*!
     * \variable Interface::preferences
     * \brief Points to an instance of the object corresponding to the preferences dialog.
     */
    Preferences * preferences;
    /*!
     * \variable Interface::sourceDialog
     * \brief Points to an instance of QFileDialog providing backup source folder selection dialog.
     */
    QFileDialog * sourceDialog,
    /*!
     * \variable Interface::targetDialog
     * \brief Points to an instance of QFileDialog providing backup destination folder selection dialog.
     */
      * targetDialog;
    /*!
     * \variable Interface::workerThread
     * \brief QThread instance which will be used to execute backup-related operations such as file copy.
     */
    QThread workerThread;
    /*!
     * \variable Interface::worker
     * \brief Points to an instance of Worker object providing all backup-related functionalities.
     */
    Worker * worker;
    /*!
     * \variable Interface::workerInProgress
     * \brief Boolean indicating whether Interface::worker performs a backup-related operation.
     */
    bool workerInProgress,
    /*!
     * \variable Interface::aborted
     * \brief Boolean indicating whether the user has requested a backup operation cancellation.
     */
      aborted;

  private:
    bool inProgress();
    QStringList ready();
    void abort();
    void loadSettings();
    void saveSettings();

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
    void onAnalysisProgress(qint64 files, qint64 directories, qint64 size);
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
     * \brief Signal telling Interface::worker to begin backup process.
     */
    void signalStart();
    /*!
     * \fn void triggerAnalysis()
     * \brief Signal telling Interface::worker to perform bacup source location contents analysis.
     */
    void triggerAnalysis();
};

#endif // __INTERFACE_H
