#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QMutex>
#include <QWaitCondition>
#include <QMessageBox>
#include <queue>
#include "item.h"
#include "producer.h"
#include "consumer.h"
#include "preferences.h"

#define KILOBYTE (qint64) 1024
#define MEGABYTE (KILOBYTE * 1024)
#define GIGABYTE (MEGABYTE * 1024)
#define TERABYTE (GIGABYTE * 1024)

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
    Producer * producer;
    Consumer * consumer;
  private slots:
    void onBrowseSource(bool clicked);
    void onBrowseTarget(bool clicked);
    void onToggleSynchronize(bool checked);
    void onToggleKeepObsolete(bool checked);
    void onEditPreferences(bool clicked);
    void onShowAboutBox(bool clicked);
    void onQuit(bool clicked);
    void onBeginBackup(bool clicked);
    void onChooseSource(QString selected);
    void onChooseTarget(QString selected);
    void onStatus(int processed, QString current);
};

#endif // INTERFACE_H
