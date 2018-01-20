#include "interface.h"
#include "ui_interface.h"

Interface::Interface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Interface),
    sourceDialog(new QFileDialog(this)),
    targetDialog(new QFileDialog(this))
{
    ui->setupUi(this);
    this->sourceDialog->setFileMode(QFileDialog::Directory);
    this->targetDialog->setFileMode(QFileDialog::Directory);
    QObject::connect(ui->browseSource, SIGNAL(clicked(bool)), this, SLOT(browseSourceSlot(bool)));
    QObject::connect(ui->browseTarget, SIGNAL(clicked(bool)), this, SLOT(browseTargetSlot(bool)));
    QObject::connect(ui->start, SIGNAL(clicked(bool)), this, SLOT(launchBackup(bool)));
    QObject::connect(this->sourceDialog, SIGNAL(fileSelected(QString)), this, SLOT(onSourceChoosen(QString)));
    QObject::connect(this->targetDialog, SIGNAL(fileSelected(QString)), this, SLOT(onTargetChoosen(QString)));
}

Interface::~Interface()
{
    delete ui;
}

void Interface::browseSourceSlot(bool clicked) {
  this->sourceDialog->show();
}

void Interface::browseTargetSlot(bool clicked) {
  this->targetDialog->show();
}

void Interface::onSourceChoosen(QString selected) {
  this->ui->sourceFolder->setText(selected);
  //this->ui->detected->setText(QString::number(this->filer->getDirectoriesToProcess() + this->filer->getFilesToProcess()));
}

void Interface::onTargetChoosen(QString selected) {
  this->ui->targetFolder->setText(selected);
}

void Interface::launchBackup(bool clicked) {

}

void Interface::onStatus(int processed, QString current) {
  //this->ui->beingProcessed->setText(current);
  //this->ui->processed->setText(QString::number(processed));
}
