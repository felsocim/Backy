#include "interface.h"
#include "ui_interface.h"

Interface::Interface(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Interface),
  preferences(new Preferences(this)),
  sourceDialog(new QFileDialog(this)),
  targetDialog(new QFileDialog(this)),
  buffer(new std::queue<Item>()),
  lock(new QMutex()),
  notEmpty(new QWaitCondition()),
  notFull(new QWaitCondition()),
  producer(new Producer()),
  consumer(new Consumer()) {
  this->ui->setupUi(this);

  this->sourceDialog->setFileMode(QFileDialog::Directory);
  this->targetDialog->setFileMode(QFileDialog::Directory);

  this->producer->setBuffer(this->buffer);
  this->producer->setLock(this->lock);
  this->producer->setNotEmpty(this->notEmpty);
  this->producer->setNotFull(this->notFull);

  QString prodEvent("prod_event.log");
  QString prodError("prod_error.log");
  this->producer->setLogger(prodEvent, prodError);

  this->consumer->setBuffer(this->buffer);
  this->consumer->setLock(this->lock);
  this->consumer->setNotEmpty(this->notEmpty);
  this->consumer->setNotFull(this->notFull);

  QString consEvent("prod_event.log");
  QString consError("prod_error.log");
  this->consumer->setLogger(consEvent, consError);


  QObject::connect(this->ui->buttonBrowseSource, SIGNAL(clicked(bool)), this, SLOT(onBrowseSource(bool)));
  QObject::connect(this->ui->buttonBrowseTarget, SIGNAL(clicked(bool)), this, SLOT(onBrowseTarget(bool)));

  QObject::connect(this->ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(onQuit(bool)));
  QObject::connect(this->ui->buttonQuit, SIGNAL(clicked(bool)), this, SLOT(onQuit(bool)));

  QObject::connect(this->sourceDialog, SIGNAL(fileSelected(QString)), this, SLOT(onChooseSource(QString)));
  QObject::connect(this->targetDialog, SIGNAL(fileSelected(QString)), this, SLOT(onChooseTarget(QString)));
}

Interface::~Interface() {
  delete ui;
}

void Interface::onBrowseSource(bool clicked) {
  this->sourceDialog->show();
}

void Interface::onBrowseTarget(bool clicked) {
  this->targetDialog->show();
}

void Interface::onQuit(bool clicked) {
  if(this->producer->isRunning() || this->consumer->isRunning()) {
    if(QMessageBox::question(
      this,
      tr("Abort and quit"),
      tr("A backup is still in progress!\nAre you sure you want to abort it and quit?"),
      QMessageBox::Cancel | QMessageBox::Abort,
      QMessageBox::Cancel
    ) == QMessageBox::Abort) {
      // TODO: Abort producer and consumer threads
    } else {
      return;
    }
  }

  this->close();
}

void Interface::onChooseSource(QString selected) {
  this->ui->editSourcePath->setText(selected);

  this->producer->setRoot(selected);
  this->consumer->setSource(selected);
  this->producer->analyze();

  this->ui->labelDiscoveredDirectoriesValue->setText(QString::number(this->producer->getDirectoriesCount()));
  this->ui->labelDiscoveredFilesValue->setText(QString::number(this->producer->getFilesCount()));

  this->consumer->setDetectedCount(this->producer->getDirectoriesCount() + this->producer->getFilesCount());

  size_t bytesDiscovered = this->producer->getSize();

  if(bytesDiscovered >= TERABYTE) {
    this->ui->labelDiscoveredTotalSizeValue->setText(
      QString::number(
        ((float) bytesDiscovered / (float) TERABYTE)
      )
    );
    this->ui->labelDiscoveredTotalSizeUnits->setText("TB");
  } else if(bytesDiscovered >= GIGABYTE) {
    this->ui->labelDiscoveredTotalSizeValue->setText(
      QString::number(
        ((float) bytesDiscovered / (float) GIGABYTE)
      )
    );
    this->ui->labelDiscoveredTotalSizeUnits->setText("GB");
  } else if(bytesDiscovered >= MEGABYTE) {
    this->ui->labelDiscoveredTotalSizeValue->setText(
      QString::number(
        ((float) bytesDiscovered / (float) MEGABYTE)
      )
    );
    this->ui->labelDiscoveredTotalSizeUnits->setText("MB");
  } else if(bytesDiscovered >= KILOBYTE) {
    this->ui->labelDiscoveredTotalSizeValue->setText(
      QString::number(
        ((float) bytesDiscovered / (float) KILOBYTE)
      )
    );
    this->ui->labelDiscoveredTotalSizeUnits->setText("kB");
  } else {
    this->ui->labelDiscoveredTotalSizeValue->setText(QString::number(bytesDiscovered));
    this->ui->labelDiscoveredTotalSizeUnits->setText("B");
  }
}

void Interface::onChooseTarget(QString selected) {
  this->ui->editTargetPath->setText(selected);
  this->consumer->setTarget(selected);
}

void Interface::launchBackup(bool clicked) {

}

void Interface::onStatus(int processed, QString current) {
  //this->ui->beingProcessed->setText(current);
  //this->ui->processed->setText(QString::number(processed));
}
