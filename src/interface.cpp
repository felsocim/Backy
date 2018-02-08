#include "../include/interface.h"
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

  this->ui->progressSourceAnalysis->hide();

  this->sourceDialog->setFileMode(QFileDialog::Directory);
  this->targetDialog->setFileMode(QFileDialog::Directory);

  this->producerInProgress = false;
  this->consumerInProgress = false;
  this->aborted = false;

  this->loadSettings();

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
  QObject::connect(this, SIGNAL(triggerAnalysis()), this->producer, SLOT(analyze()));
  QObject::connect(this->producer, SIGNAL(triggerAnalysisProgress(qint64,qint64,qint64)), this, SLOT(onAnalysisProgress(qint64,qint64,qint64)));
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

  QObject::connect(this->consumer, SIGNAL(triggerCurrentOperation(QString)), this, SLOT(onStatusCurrentOperation(QString)));
  QObject::connect(this->consumer, SIGNAL(triggerCurrentItem(QString)), this, SLOT(onStatusCurrentItem(QString)));
  QObject::connect(this->consumer, SIGNAL(triggerCurrentProgress(int)), this, SLOT(onStatusCurrentProgress(int)));
  QObject::connect(this->consumer, SIGNAL(triggerOverallProgress(int)), this, SLOT(onStatusOverallProgress(int)));

  QObject::connect(this->ui->actionPreferences, SIGNAL(triggered(bool)), this, SLOT(onEditPreferences(bool)));
  QObject::connect(this->ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(onShowAboutBox(bool)));
  QObject::connect(this->ui->actionDocumentation, SIGNAL(triggered(bool)), this, SLOT(onDocumentationSolicitation(bool)));

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

QStringList Interface::ready() {
  QStringList result;

  if(this->ui->editSourcePath->text().isEmpty())
    result << QString(tr("- No source drive or folder provided!"));
  else {
    QDir source(this->ui->editSourcePath->text());

    if(!source.exists())
      result << QString(tr("- Source drive or folder does not exists!"));
  }

  if(this->ui->editTargetPath->text().isEmpty())
    result << QString(tr("- No target drive or folder provided!"));
  else {
    QDir target(this->ui->editTargetPath->text());

    if(!target.exists())
      result << QString(tr("- Target drive or folder does not exists!"));
    else if(this->producer->getDirectoriesCount() + this->producer->getFilesCount() < 1)
      result << QString(tr("- Source drive or folder does not contain any files or folders!"));
  }

  return result;
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

  this->producer->setItemBufferSize(itemBufferSize);
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
  QMessageBox::about(
    this,
    tr("About Backy"),
    tr(
      "<h1>Backy</h1>"
      "<p>Backy is a simple open-source tool to create and maintain backup of a folder or an entire drive.</p>"
      "<p>Copyright (C) 2018 Marek Felsoci</p>"
      "<p>This program is free software: you can redistribute it and/or modify"
      " it under the terms of the GNU General Public License as published by"
      " the Free Software Foundation, either version 3 of the License, or"
      " any later version."
      "<p>This program is distributed in the hope that it will be useful,"
      " but WITHOUT ANY WARRANTY; without even the implied warranty of"
      " MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.</p>"
      "<p>See the GNU General Public License for more details."
      " You should have received a copy of the GNU General Public License"
      " along with this program. If not, see <a href='http://www.gnu.org/licenses/' target='_blank'>http://www.gnu.org/licenses/</a></p>"
      "<p><strong>Information:</strong> For program's documentation, source code and updates, please, refer to its repository at: <a href='https://github.com/felsocim/Backy' target='_blank'>https://github.com/felsocim/Backy</a></p>"
      "<p><strong>Credits:</strong> The <em>Fantasque Sans Mono</em> font was created by <a href='mailto:jany.belluz@hotmail.fr' target='_blank'>"
      "Jany Belluz</a> and is licensed under the terms of <a href='http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL'>"
      "SIL Open Font License</a>. The <em>Abstract Materials Patterns</em> were freely acquired from <a href='https://www.brusheezy.com' target='_blank'>Brusheezy!</a>"
    )
  );
}

void Interface::onAbort(bool clicked) {
  if(QMessageBox::question(
    this,
    tr("Abort requested"),
    tr("Backup is still in progress!\nNote that on confirmation the backup process will be aborted once the backup of the current item will be finished.\nAre you sure you want to abort the backup process?"),
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
      tr("Backup is still in progress!\nAre you sure you want to abort it and quit?"),
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

void Interface::onBeginBackup(bool clicked) {
  if(this->inProgress()) {
    QMessageBox::critical(
      this,
      tr("Already in progress"),
      tr("Another action is already in progress!\nPlease wait until it's finished."),
      QMessageBox::Ok
    );
    return;
  }

  QStringList check = this->ready();

  if(!check.isEmpty()) {
    QMessageBox::critical(
      this,
      tr("Wrong parameters"),
      tr("Some backup parameters are unset or set incorrectly!\nPlease verify them before continuing.\nError report:\n") + check.join('\n'),
      QMessageBox::Ok
    );
    return;
  }

  if(QMessageBox::question(
    this,
    tr("Ready"),
    tr("Backup is ready to be performed.\nPlease, verify all backup parameters before you continue.\nAre you sure you want to begin the backup process?"),
    QMessageBox::Yes | QMessageBox::No
  ) == QMessageBox::Yes) {
    this->ui->buttonBackup->setEnabled(false);
    this->producer->setProgress(true);
    this->consumer->setProgress(true);
    this->ui->buttonAbort->setEnabled(true);
    emit this->signalStart();
  }
}

void Interface::onChooseSource(QString selected) {
  this->ui->progressSourceAnalysis->setMaximum(0);
  this->ui->progressSourceAnalysis->show();
  this->ui->editSourcePath->setText(selected);
  this->producer->setRoot(selected);
  this->consumer->setSource(selected);
  emit this->triggerAnalysis();
}

void Interface::onAnalysisProgress(qint64 files, qint64 directories, qint64 size) {
  this->ui->labelDiscoveredDirectoriesValue->setText(QString::number(directories));
  this->ui->labelDiscoveredFilesValue->setText(QString::number(files));

  double converted = 0.0;

  if(size >= TERABYTE) {
    converted = (double) size / (double) TERABYTE;
    this->ui->labelDiscoveredTotalSizeUnits->setText(tr("TB"));
  } else if(size >= GIGABYTE) {
    converted = (double) size / (double) GIGABYTE;
    this->ui->labelDiscoveredTotalSizeUnits->setText(tr("GB"));
  } else if(size >= MEGABYTE) {
    converted = (double) size / (double) MEGABYTE;
    this->ui->labelDiscoveredTotalSizeUnits->setText(tr("MB"));
  } else if(size >= KILOBYTE) {
    converted = (double) size / (double) KILOBYTE;
    this->ui->labelDiscoveredTotalSizeUnits->setText(tr("kB"));
  } else {
    converted = (double) size;
    this->ui->labelDiscoveredTotalSizeUnits->setText(tr("B"));
  }

  this->ui->labelDiscoveredTotalSizeValue->setText(QString::number(converted, 'g', 2));
}

void Interface::onChooseTarget(QString selected) {
  this->ui->editTargetPath->setText(selected);
  this->consumer->setTarget(selected);
}

void Interface::onStatusCurrentOperation(QString operation) {
  this->ui->labelStatusCurrentOperation->setText(operation);
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
  this->consumer->setDetectedCount(this->producer->getDirectoriesCount() + this->producer->getFilesCount());
  this->consumer->setDetectedSize(this->producer->getSize());
  this->ui->progressSourceAnalysis->setMaximum(1);
  this->ui->progressSourceAnalysis->hide();
  this->producerInProgress = false;
}

void Interface::onConsumerFinished() {
  this->consumerInProgress = false;
  this->ui->labelStatusCurrentOperation->setText("");
  this->ui->labelStatusCurrentName->setText("");
  this->ui->progressStatusCurrentProgress->setValue(0);
  this->ui->progressStatusOverallProgress->setValue(0);

  if(this->aborted) {
    this->aborted = false;
    QMessageBox::warning(
      this,
      tr("Backup aborted"),
      tr("Backup process has been aborted by the user!"),
      QMessageBox::Ok
    );
  } else if(this->consumer->didErrorOccurred()) {
    QMessageBox::warning(
      this,
      tr("Backup complete with errors"),
      tr("Backup completed but errors occurred during the process! Check the log file in '%1' for more details.\n").arg(this->preferences->getLogsLocation()),
      QMessageBox::Ok
    );
  } else {
    QMessageBox::information(
      this,
      tr("Backup complete"),
      tr("Backup completed succefully!"),
      QMessageBox::Ok
    );
  }

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

void Interface::onDocumentationSolicitation(bool clicked) {
  if(!QDesktopServices::openUrl(QUrl("https://github.com/felsocim/Backy"))) {
    QMessageBox::critical(
      this,
      tr("Browser unavailable"),
      tr("Unable to open the online documentation because of unavailable browser!\nPlease, check if there is a default browser set in your system and try again.\nOtherwise refer to https://github.com/felsocim/Backy for the documentation."),
      QMessageBox::Ok
    );
  }
}
