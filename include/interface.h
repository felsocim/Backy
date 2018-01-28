#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QMessageBox>
#include <QSettings>
#include <queue>
#include "item.h"
#include "producer.h"
#include "consumer.h"
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
    std::queue<Item> * buffer;
    QMutex * lock;
    QWaitCondition * notEmpty,
      * notFull;
    QThread producerWorker,
      consumerWorker;
    Producer * producer;
    Consumer * consumer;
    bool producerInProgress,
      consumerInProgress;
    void abort();
    bool inProgress();
    void loadSettings();
    void saveSettings();
  private slots:
    void onBrowseSource(bool clicked);
    void onBrowseTarget(bool clicked);
    void onToggleSynchronize(bool checked);
    void onToggleKeepObsolete(bool checked);
    void onToggleCriterionMostRecent(bool checked);
    void onToggleCriterionBiggest(bool checked);
    void onEditPreferences(bool clicked);
    void onShowAboutBox(bool clicked);
    void onAbort(bool clicked);
    void onQuit(bool clicked);
    void onBeginBackup(bool clicked);
    void onChooseSource(QString selected);
    void onChooseTarget(QString selected);
    void onStatusCurrentItem(QString item);
    void onStatusCurrentProgress(int current);
    void onStatusOverallProgress(int overall);
    void onProducerStarted();
    void onConsumerStarted();
    void onProducerFinished();
    void onConsumerFinished();
    void onSavePreferences();
  signals:
    void signalStart();
    void signalProgress(bool state);
};

#endif // INTERFACE_H
