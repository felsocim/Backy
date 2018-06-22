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

class Interface : public QMainWindow {
  Q_OBJECT
  public:
    explicit Interface(QWidget * parent = 0);
    ~Interface();

  private:
    Ui::Interface * ui;
    Preferences * preferences;
    QFileDialog * sourceDialog,
      * targetDialog;
    QThread workerThread;
    Worker * worker;
    bool workerInProgress,
      aborted;
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
    void onAnalysisProgress(quint64 files, quint64 directories, quint64 size);
    void onChooseTarget(QString selected);
    void onStatusCurrentOperation(QString operation);
    void onStatusCurrentItem(QString item);
    void onStatusCurrentProgress(int current);
    void onStatusOverallProgress(int overall);
    void onWorkerStarted();
    void onWorkerFinished();
    void onSavePreferences();
    void onDocumentationSolicitation();

  signals:
    void signalStart();
    void signalProgress(bool state);
    void triggerAnalysis();
};

#endif // __INTERFACE_H
