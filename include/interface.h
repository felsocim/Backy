#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>

namespace Ui {
  class Interface;
}

class Interface : public QMainWindow {
  Q_OBJECT

  public:
    explicit Interface(QWidget *parent = 0);
    ~Interface();

  private:
    Ui::Interface * ui;
    QFileDialog * sourceDialog,
      * targetDialog;
  private slots:
    void browseSourceSlot(bool clicked);
    void browseTargetSlot(bool clicked);
    void launchBackup(bool clicked);
    void onSourceChoosen(QString selected);
    void onTargetChoosen(QString selected);
    void onStatus(int processed, QString current);
};

#endif // INTERFACE_H
