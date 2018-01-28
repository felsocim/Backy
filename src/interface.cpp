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
  this->producerInProgress = false;
  this->consumerInProgress = false;
  this->aborted = false;
  this->loadSettings();

  this->sourceDialog->setFileMode(QFileDialog::Directory);
  this->targetDialog->setFileMode(QFileDialog::Directory);

  this->producer->setBuffer(this->buffer);
  this->producer->setLock(this->lock);
  this->producer->setNotEmpty(this->notEmpty);
  this->producer->setNotFull(this->notFull);
  this->producer->createLogsAt(this->preferences->getLogsLocation());

  this->consumer->setBuffer(this->buffer);
  this->consumer->setLock(this->lock);
  this->consumer->setNotEmpty(this->notEmpty);
  this->consumer->setNotFull(this->notFull);
  this->consumer->createLogsAt(this->preferences->getLogsLocation());

  this->producer->moveToThread(&this->producerWorker);
  this->consumer->moveToThread(&this->consumerWorker);

  QObject::connect(this, SIGNAL(signalStart()), this->producer, SLOT(doWork()));
  QObject::connect(this, SIGNAL(signalStart()), this->consumer, SLOT(doWork()));

  QObject::connect(this->producer, SIGNAL(started()), this, SLOT(onProducerStarted()));
  QObject::connect(this->producer, SIGNAL(finished()), this, SLOT(onProducerFinished()));
  QObject::connect(this->consumer, SIGNAL(started()), this, SLOT(onConsumerStarted()));
  QObject::connect(this->consumer, SIGNAL(finished()), this, SLOT(onConsumerFinished()));

  QObject::connect(this->ui->buttonBrowseSource, SIGNAL(clicked(bool)), this, SLOT(onBrowseSource(bool)));
  QObject::connect(this->ui->buttonBrowseTarget, SIGNAL(clicked(bool)), this, SLOT(onBrowseTarget(bool)));

  QObject::connect(this->ui->checkSynchronize, SIGNAL(toggled(bool)), this, SLOT(onToggleSynchronize(bool)));
  QObject::connect(this->ui->checkKeepObsolete, SIGNAL(toggled(bool)), this, SLOT(onToggleKeepObsolete(bool)));
  QObject::connect(this->ui->radioCriterionMostRecent, SIGNAL(toggled(bool)), this, SLOT(onToggleCriterionMostRecent(bool)));
  QObject::connect(this->ui->radioCriterionBiggest, SIGNAL(toggled(bool)), this, SLOT(onToggleCriterionBiggest(bool)));

  QObject::connect(this->ui->buttonAbort, SIGNAL(clicked(bool)), this, SLOT(onAbort(bool)));
  QObject::connect(this->ui->buttonBackup, SIGNAL(clicked(bool)), this, SLOT(onBeginBackup(bool)));

  QObject::connect(this->consumer, SIGNAL(currentItem(QString)), this, SLOT(onStatusCurrentItem(QString)));
  QObject::connect(this->consumer, SIGNAL(currentProgress(int)), this, SLOT(onStatusCurrentProgress(int)));
  QObject::connect(this->consumer, SIGNAL(overallProgress(int)), this, SLOT(onStatusOverallProgress(int)));

  QObject::connect(this->ui->actionPreferences, SIGNAL(triggered(bool)), this, SLOT(onEditPreferences(bool)));
  QObject::connect(this->ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(onShowAboutBox(bool)));

  QObject::connect(this->ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(onQuit(bool)));
  QObject::connect(this->ui->buttonQuit, SIGNAL(clicked(bool)), this, SLOT(onQuit(bool)));

  QObject::connect(this->sourceDialog, SIGNAL(fileSelected(QString)), this, SLOT(onChooseSource(QString)));
  QObject::connect(this->targetDialog, SIGNAL(fileSelected(QString)), this, SLOT(onChooseTarget(QString)));

  QObject::connect(this->preferences, SIGNAL(triggerSave()), this, SLOT(onSavePreferences()));

  this->producerWorker.start();
  this->consumerWorker.start();
}

Interface::~Interface() {
  this->producerWorker.quit();
  this->consumerWorker.quit();
  this->producerWorker.wait();
  this->consumerWorker.wait();
  this->saveSettings();
  delete this->ui;
  delete this->preferences;
  delete this->sourceDialog;
  delete this->targetDialog;
  delete this->buffer;
  delete this->lock;
  delete this->notEmpty;
  delete this->notFull;
  delete this->producer;
  delete this->consumer;
}

bool Interface::inProgress() {
  return this->producerInProgress || this->consumerInProgress;
}

void Interface::abort() {
  this->producer->setProgress(false);
  this->consumer->setProgress(false);
  this->aborted = true;
}

void Interface::loadSettings() {
  QSettings settings;
  settings.beginGroup("Backup");
  bool synchronize = settings.value("synchronize", DEFAULT_SYNCHRONIZE).toBool();
  bool keepObsolete = settings.value("keepObsolete", DEFAULT_KEEP_OBSOLETE).toBool();
  Criterion comparisonCriterion = (Criterion) settings.value("comparisonCriterion", DEFAULT_COMPARISON_CRITERION).toInt();
  this->consumer->setSynchronize(synchronize);
  this->ui->checkSynchronize->setChecked(synchronize);
  this->consumer->setKeepObsolete(keepObsolete);
  this->ui->checkKeepObsolete->setChecked(keepObsolete);
  switch(comparisonCriterion) {
    case CRITERION_MORE_RECENT:
    default:
      this->onToggleCriterionMostRecent(true);
      break;
    case CRITERION_BIGGER:
      this->onToggleCriterionBiggest(true);
      break;
  }
  settings.endGroup();
  settings.beginGroup("Application");
  qint64 itemBufferSize = settings.value("itemBufferSize", DEFAULT_ITEM_BUFFER_SIZE).toLongLong();
  qint64 copyBufferSize = settings.value("copyBufferSize", DEFAULT_COPY_BUFFER_SIZE).toLongLong();
  QString logsLocation = settings.value("logsLocation", DEFAULT_LOGS_LOCATION).toString();
  this->producer->setBufferMax(itemBufferSize);
  this->preferences->setItemBufferSize(itemBufferSize);
  this->consumer->setCopyBufferSize(copyBufferSize);
  this->preferences->setCopyBufferSize(copyBufferSize);
  this->preferences->setLogsLocation(logsLocation);
  settings.endGroup();
}

void Interface::saveSettings() {
  QSettings settings;
  settings.beginGroup("Backup");
  settings.setValue("synchronize", this->ui->checkSynchronize->isChecked());
  settings.setValue("keepObsolete", this->ui->checkKeepObsolete->isChecked());
  settings.setValue("comparisonCriterion", (this->ui->radioCriterionMostRecent->isChecked() ? CRITERION_MORE_RECENT : CRITERION_BIGGER));
  settings.endGroup();
  settings.beginGroup("Application");
  settings.setValue("itemBufferSize", this->preferences->getItemBufferSize());
  settings.setValue("copyBufferSize", this->preferences->getCopyBufferSize());
  settings.setValue("logsLocation", this->preferences->getLogsLocation());
  settings.endGroup();
}

void Interface::onBrowseSource(bool clicked) {
  this->sourceDialog->show();
}

void Interface::onBrowseTarget(bool clicked) {
  this->targetDialog->show();
}

void Interface::onToggleSynchronize(bool checked) {
  this->consumer->setSynchronize(checked);
}

void Interface::onToggleKeepObsolete(bool checked) {
  this->consumer->setKeepObsolete(checked);
}

void Interface::onToggleCriterionMostRecent(bool checked) {
  this->ui->radioCriterionMostRecent->setChecked(checked);
  this->ui->radioCriterionBiggest->setChecked(!checked);
  this->consumer->setCriterion(CRITERION_MORE_RECENT);
}

void Interface::onToggleCriterionBiggest(bool checked) {
  this->ui->radioCriterionBiggest->setChecked(checked);
  this->ui->radioCriterionMostRecent->setChecked(!checked);
  this->consumer->setCriterion(CRITERION_BIGGER);
}

void Interface::onEditPreferences(bool clicked) {
  this->preferences->show();
}

void Interface::onShowAboutBox(bool clicked) {
  QMessageBox::about(this, "About Backy", tr("Backy is a simple backup program allowing to create and synchronize drive or folder backups.\nCopyright (C) 2018 Marek Felsoci. Lincesed under LGPL License."));
}

void Interface::onAbort(bool clicked) {
  if(QMessageBox::question(
    this,
    tr("Abort requested"),
    tr("Backup is still in progress!\nNote that if you click on 'Yes' the backup process will be aborted when the backup of the current item will be finished.\nAre you sure you want to abort the backup process?"),
    QMessageBox::Yes | QMessageBox::No
  ) == QMessageBox::Yes) {
    this->abort();
  }
}

void Interface::onQuit(bool clicked) {
  if(this->inProgress()) {
    if(QMessageBox::question(
      this,
      tr("Abort and quit"),
      tr("A backup is still in progress!\nAre you sure you want to abort it and quit?"),
      QMessageBox::Cancel | QMessageBox::Abort,
      QMessageBox::Cancel
    ) == QMessageBox::Abort) {
      this->abort();
    } else {
      return;
    }
  }

  QApplication::quit();
}

void Interface::onChooseSource(QString selected) {
  this->ui->editSourcePath->setText(selected);

  this->producer->setRoot(selected);
  this->consumer->setSource(selected);
  this->producer->analyze();

  this->ui->labelDiscoveredDirectoriesValue->setText(QString::number(this->producer->getDirectoriesCount()));
  this->ui->labelDiscoveredFilesValue->setText(QString::number(this->producer->getFilesCount()));

  this->consumer->setDetectedCount(this->producer->getDirectoriesCount() + this->producer->getFilesCount());
  this->consumer->setDetectedSize(this->producer->getSize());

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

void Interface::onBeginBackup(bool clicked) {
  if(this->producer->getDirectoriesCount() + this->producer->getFilesCount() > 0 && !this->inProgress()) {
    this->producer->setProgress(true);
    this->consumer->setProgress(true);
    emit this->signalStart();
  }
}

void Interface::onStatusCurrentItem(QString item) {
  this->ui->labelStatusCurrentName->setText(item);
}

void Interface::onStatusCurrentProgress(int current) {
  this->ui->progressStatusCurrentProgress->setValue(current);
}

void Interface::onStatusOverallProgress(int overall) {
  this->ui->progressStatusOverallProgress->setValue(overall);
}

void Interface::onProducerStarted() {
  this->producerInProgress = true;
}

void Interface::onConsumerStarted() {
  this->consumerInProgress = true;
}

void Interface::onProducerFinished() {
  this->producerInProgress = false;
}

void Interface::onConsumerFinished() {
  this->consumerInProgress = false;
  this->ui->labelStatusCurrentName->setText("");
  this->ui->progressStatusCurrentProgress->setValue(0);
  this->ui->progressStatusOverallProgress->setValue(0);

  this->ui->buttonBackup->setEnabled(true);
  this->ui->buttonAbort->setEnabled(false);
}

void Interface::onSavePreferences() {
  if(this->inProgress()) {
    QMessageBox::warning(
      this,
      tr("Unable to save preferences"),
      tr("A backup is currently in progress!\nThus the changes you've made to application's preferences cannot be applied!\nPlease redo your changes once the backup will be finished."),
      QMessageBox::Ok
    );
  } else {
    this->saveSettings();
  }
}
  }
}
