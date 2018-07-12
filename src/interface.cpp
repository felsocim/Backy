/*!
 * This file is a part of Backy project, a simple backup creation and
 * maintaining solution.
 * 
 * Copyright (C) 2018 Marek Felsoci <marek.felsoci@gmail.com> (Feldev)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * \file interface.cpp
 * \brief Contains definitions related to Interface class.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0
 */
#include "../include/interface.h"
#include "ui_interface.h"

/*!
 * \fn Interface::Interface(QWidget *parent)
 * \param parent Pointer to parent widget.
 * \brief Constructor of Interface class.
 */
Interface::Interface(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Interface),
  preferences(new Preferences(this)),
  sourceDialog(new QFileDialog(this)),
  targetDialog(new QFileDialog(this)),
  worker(new Worker()) {
  /* Setup the graphical user interface of the application */
  this->ui->setupUi(this);

  this->ui->progressSourceAnalysis->hide();

  this->sourceDialog->setFileMode(QFileDialog::Directory);
  this->targetDialog->setFileMode(QFileDialog::Directory);

  /* Initialize class members with their default values */
  this->workerInProgress = false;
  this->aborted = false;

  /* Attempt to load application's settings */
  this->loadSettings();

  /* Create logs at the preferred location */
  this->worker->createLogsAt(this->preferences->getLogsLocation());

  /* Execute all backup-related operations in a separate thread */
  this->worker->moveToThread(&this->workerThread);

  /* Connect all signals to their respective slots */
  QObject::connect(this, SIGNAL(signalStart()), this->worker,
    SLOT(doWork()));
  QObject::connect(this, SIGNAL(triggerAnalysis()), this->worker,
    SLOT(analyze()));

  QObject::connect(this->worker, SIGNAL(started()), this,
    SLOT(onWorkerStarted()));
  QObject::connect(this->worker, SIGNAL(finished(int)), this,
    SLOT(onWorkerFinished(int)));
  QObject::connect(this->worker, SIGNAL(triggerAnalysisProgress(qint64,
    qint64, qint64)), this,
    SLOT(onAnalysisProgress(qint64,qint64,qint64)));

  QObject::connect(this->ui->buttonBrowseSource, SIGNAL(clicked(bool)),
    this, SLOT(onBrowseSource()));
  QObject::connect(this->ui->buttonBrowseTarget, SIGNAL(clicked(bool)),
    this, SLOT(onBrowseTarget()));

  QObject::connect(this->ui->checkSynchronize, SIGNAL(toggled(bool)),
    this, SLOT(onToggleSynchronize(bool)));
  QObject::connect(this->ui->checkKeepObsolete, SIGNAL(toggled(bool)),
    this, SLOT(onToggleKeepObsolete(bool)));
  QObject::connect(this->ui->radioCriterionMostRecent,
    SIGNAL(toggled(bool)), this,
    SLOT(onToggleCriterionMostRecent(bool)));
  QObject::connect(this->ui->radioCriterionBiggest,
    SIGNAL(toggled(bool)), this,
    SLOT(onToggleCriterionBiggest(bool)));

  QObject::connect(this->ui->buttonAbort, SIGNAL(clicked(bool)),
    this, SLOT(onAbort()));
  QObject::connect(this->ui->buttonBackup, SIGNAL(clicked(bool)),
    this, SLOT(onBeginBackup()));

  QObject::connect(this->worker,
    SIGNAL(triggerCurrentOperation(QString)), this,
    SLOT(onStatusCurrentOperation(QString)));
  QObject::connect(this->worker, SIGNAL(triggerCurrentItem(QString)),
    this, SLOT(onStatusCurrentItem(QString)));
  QObject::connect(this->worker, SIGNAL(triggerCurrentProgress(int)),
    this, SLOT(onStatusCurrentProgress(int)));
  QObject::connect(this->worker, SIGNAL(triggerOverallProgress(int)),
    this, SLOT(onStatusOverallProgress(int)));

  QObject::connect(this->ui->actionPreferences, SIGNAL(triggered(bool)),
    this, SLOT(onEditPreferences()));
  QObject::connect(this->ui->actionAbout, SIGNAL(triggered(bool)), this,
    SLOT(onShowAboutBox()));
  QObject::connect(this->ui->actionDocumentation,
    SIGNAL(triggered(bool)), this, SLOT(onDocumentationSolicitation()));

  QObject::connect(this->ui->actionQuit, SIGNAL(triggered(bool)), this,
    SLOT(onQuit()));
  QObject::connect(this->ui->buttonQuit, SIGNAL(clicked(bool)), this,
    SLOT(onQuit()));

  QObject::connect(this->sourceDialog, SIGNAL(fileSelected(QString)),
    this, SLOT(onChooseSource(QString)));
  QObject::connect(this->targetDialog, SIGNAL(fileSelected(QString)),
    this, SLOT(onChooseTarget(QString)));

  QObject::connect(this->preferences, SIGNAL(triggerSave()), this,
    SLOT(onSavePreferences()));

  /* Start the backup-related operations dedicated thread */
  this->workerThread.start();
}

/*!
 * \fn Interface::~Interface()
 * \brief Destructor of Interface class.
 */
Interface::~Interface() {
  /* Safely terminate the backup-related operations dedicated thread */
  this->workerThread.quit();
  this->workerThread.wait();

  /* 
   * Save the application's preferences as they might have been modified
   * by the user
   */
  this->saveSettings();

  delete this->ui;
  delete this->preferences;
  delete this->sourceDialog;
  delete this->targetDialog;
  delete this->worker;
}

/*!
 * \fn bool Interface::inProgress()
 * \returns \c true if a backup-related operation is currently in
 *          progress.
 * \returns \c false otherwise.
 */
bool Interface::inProgress() {
  return this->workerInProgress;
}

/*!
 * \fn QStringList Interface::ready()
 * \brief Verify if all conditions necessary for a backup to be
 *        performed correctly are met.
 * \returns QStringList of conditions that have not been satisfied yet.
 * \returns An empty QStringList if all of conditions have been
 *          satisfied.
 */
QStringList Interface::ready() {
  QStringList result;

  /* Check the backup source folder */
  if(this->ui->editSourcePath->text().isEmpty())
    result << QString(tr("- No source drive or folder provided!"));
  else {
    QDir source(this->ui->editSourcePath->text());

    if(!source.exists())
      result << QString(tr("- Source drive or folder does not exists!"));
  }

  /* Check the backup destination folder */
  if(this->ui->editTargetPath->text().isEmpty())
    result << QString(tr("- No target drive or folder provided!"));
  else {
    QDir target(this->ui->editTargetPath->text());

    if(!target.exists())
      result << QString(tr("- Target drive or folder does not"
      "exists!"));
    else if(this->worker->getDirectoriesCount() +
      this->worker->getFilesCount() < 1)
      result << QString(tr("- Source drive or folder does not contain"
        "any files or folders!"));
  }

  return result;
}

/*!
 * \fn void Interface::abort()
 * \brief Propagates user's request of current backup operation
 *        cancellation.
 */
void Interface::abort() {
  this->worker->setProgress(false);
  this->aborted = true;
}

/*!
 * \fn void Interface::loadSettings()
 * \brief Attempts to load previously saved application's preferences if
 *        any.
 * 
 * If a preference value cannot be loaded, the default value will be
 * used.
 */
void Interface::loadSettings() {
  /* 
   * Get preferences values (from the Registry database on Windows or
   * from a configuration file on Linux)
   */
  QSettings settings;

  settings.beginGroup("Backup");

  bool synchronize = settings.value("synchronize",
    DEFAULT_SYNCHRONIZE).toBool();
  bool keepObsolete = settings.value("keepObsolete",
    DEFAULT_KEEP_OBSOLETE).toBool();
  Criterion comparisonCriterion = (Criterion) 
    settings.value("comparisonCriterion",
    DEFAULT_COMPARISON_CRITERION).toInt();

  this->worker->setSynchronize(synchronize);
  this->ui->checkSynchronize->setChecked(synchronize);
  this->worker->setKeepObsolete(keepObsolete);
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

  quint64 copyBufferSize = settings.value("copyBufferSize",
    DEFAULT_COPY_BUFFER_SIZE).toLongLong();
  QString logsLocation = settings.value("logsLocation",
    DEFAULT_LOGS_LOCATION).toString();
  QVariant locale = settings.value("locale", DEFAULT_LOCALE_CODE);

  /* Propagate gathered values */
  this->worker->setCopyBufferSize(copyBufferSize);
  this->preferences->setCopyBufferSize(copyBufferSize);
  this->preferences->setLogsLocation(logsLocation);
  this->preferences->setLocale(locale);

  /* Cleaning */
  settings.endGroup();
}

/*!
 * \fn void Interface::saveSettings()
 * \brief Saves current application's preferences values.
 */
void Interface::saveSettings() {
  QSettings settings;

  settings.beginGroup("Backup");
  settings.setValue("synchronize",
    this->ui->checkSynchronize->isChecked());
  settings.setValue("keepObsolete",
    this->ui->checkKeepObsolete->isChecked());
  settings.setValue("comparisonCriterion",
    (this->ui->radioCriterionMostRecent->isChecked() ?
      CRITERION_MORE_RECENT : CRITERION_BIGGER));
  settings.endGroup();

  settings.beginGroup("Application");
  settings.setValue("copyBufferSize",
    this->preferences->getCopyBufferSize());
  settings.setValue("logsLocation",
    this->preferences->getLogsLocation());
  settings.setValue("locale",
    this->preferences->getLocale());
  settings.endGroup();
}

/*!
 * \fn void Interface::toggleControlsState(bool actionInProgress)
 * \brief Toggles the visibility of selected user interface controls depending
 *        on the current state of the Worker (i.e. deactivates the process
 *        cancellation button when no action is in progress and so on).
 * \param actionInProgress Boolean indicating whether the Worker currently
 *                         performs an action or not.
 */
void Interface::toggleControlsState(bool actionInProgress) {
  this->ui->buttonBrowseSource->setEnabled(!actionInProgress);
  this->ui->buttonBrowseTarget->setEnabled(!actionInProgress);
  this->ui->radioCriterionBiggest->setEnabled(!actionInProgress);
  this->ui->radioCriterionMostRecent->setEnabled(!actionInProgress);
  this->ui->checkKeepObsolete->setEnabled(!actionInProgress);
  this->ui->checkSynchronize->setEnabled(!actionInProgress);
  this->ui->buttonBackup->setEnabled(!actionInProgress);
  this->ui->buttonAbort->setEnabled(actionInProgress);
  this->ui->actionPreferences->setEnabled(!actionInProgress);
}

/*!
 * \fn void Interface::onBrowseSource()
 * \brief Slot that launches backup source location selection dialog.
 */
void Interface::onBrowseSource() {
  this->sourceDialog->show();
}

/*!
 * \fn void Interface::onBrowseTarget()
 * \brief Slot that launches backup destination location selection
 *        dialog.
 */
void Interface::onBrowseTarget() {
  this->targetDialog->show();
}

/*!
 * \fn void Interface::onToggleSynchronize(bool checked)
 * \param checked Current check status.
 * \brief Slot that toggles the \b Synchronize \b only check button.
 */
void Interface::onToggleSynchronize(bool checked) {
  this->worker->setSynchronize(checked);
}

/*!
 * \fn void Interface::onToggleKeepObsolete(bool checked)
 * \param checked Current check status.
 * \brief Slot that toggles the \b Keep \b obsolete check button.
 */
void Interface::onToggleKeepObsolete(bool checked) {
  this->worker->setKeepObsolete(checked);
}

/*!
 * \fn void Interface::onToggleCriterionMostRecent(bool checked)
 * \param checked Current check status.
 * \brief Slot that checks the \b Prefer \b more \b recent radio button
 *        and unchecks the \b Prefer \b bigger} radio button and
 *        vice-versa.
 */
void Interface::onToggleCriterionMostRecent(bool checked) {
  this->ui->radioCriterionMostRecent->setChecked(checked);
  this->ui->radioCriterionBiggest->setChecked(!checked);
  this->worker->setCriterion(CRITERION_MORE_RECENT);
}

/*!
 * \fn void Interface::onToggleCriterionBiggest(bool checked)
 * \param checked Current check status.
 * \brief Slot that checks the \b Prefer \b bigger radio button and
 *        unchecks the \b Prefer \b more \b recent radio button and 
 *        vice-versa.
 */
void Interface::onToggleCriterionBiggest(bool checked) {
  this->ui->radioCriterionBiggest->setChecked(checked);
  this->ui->radioCriterionMostRecent->setChecked(!checked);
  this->worker->setCriterion(CRITERION_BIGGER);
}

/*!
 * \fn void Interface::onEditPreferences()
 * \brief Slot that shows application's Preferences dialog.
 */
void Interface::onEditPreferences() {
  this->preferences->show();
}

/*!
 * \fn void Interface::onShowAboutBox()
 * \brief Slot that shows application's \b About \b box.
 */
void Interface::onShowAboutBox() {
  QMessageBox::about(
    this,
    tr("About Backy"),
    tr(
      "<h1>Backy</h1>"
      "<p>Backy is a simple open-source tool to create and maintain"
      "backup of a folder or an entire drive.</p>"
      "<p>Copyright (C) 2018 Marek Felsoci</p>"
      "<p>This program is free software: you can redistribute it and/or"
      " modify it under the terms of the GNU General Public License as"
      " published by the Free Software Foundation, either version 3 of"
      " the License, or (at your option) any later version."
      "<p>This program is distributed in the hope that it will be"
      " useful, but WITHOUT ANY WARRANTY; without even the implied"
      " warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR"
      " PURPOSE.</p>"
      "<p>See the GNU General Public License for more details. You"
      " should have received a copy of the GNU General Public License"
      " along with this program. If not, see"
      " <a href='http://www.gnu.org/licenses/' target='_blank'>"
      "http://www.gnu.org/licenses/</a></p>"
      "<p><strong>Information:</strong> For program's documentation,"
      " source code and updates, please, refer to its repository at:"
      " <a href='https://github.com/felsocim/Backy' target='_blank'>"
      "https://github.com/felsocim/Backy</a></p>"
      "<p><strong>Credits:</strong> The <em>Tango</em> icon theme used"
      " in this application was freely acquired from"
      " <a href='http://tango.freedesktop.org/Tango_Desktop_Project'"
      " target='_blank'>Tango Desktop Project</a>."
    )
  );
}

/*!
 * \fn void Interface::onAbort()
 * \brief Slot triggered if user requests the cancellation of current
 *        backup-related operation
 */
void Interface::onAbort() {
  if(QMessageBox::question(
    this,
    tr("Abort requested"),
    tr("An action is still in progress!\nNote that on confirmation the"
      " action will be aborted once the backup of the current"
      " item will be finished.\nAre you sure you want to abort the"
      " action in progress?"),
    QMessageBox::Yes | QMessageBox::No
  ) == QMessageBox::Yes) {
    this->ui->buttonAbort->setEnabled(false);
    this->abort();
  }
}

/*!
 * \fn void Interface::onQuit()
 * \brief Slot triggered when user chooses to quit the application.
 */
void Interface::onQuit() {
  /*
   * Check if there is a backup still in progress before exiting the
   * application
   */
  if(this->inProgress()) {
    if(QMessageBox::question(
      this,
      tr("Abort and quit"),
      tr("Backup is still in progress!\nAre you sure you want to abort"
        " it and quit?"),
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

/*!
 * \fn void Interface::onBeginBackup()
 * \brief Slot triggered when user launches a backup.
 */
void Interface::onBeginBackup() {
  /*
   * Check if there is a backup still in progress before launching a new
   * one
   */
  if(this->inProgress()) {
    QMessageBox::critical(
      this,
      tr("Already in progress"),
      tr("Another action is already in progress!\nPlease wait until"
        " it's finished."),
      QMessageBox::Ok
    );
    return;
  }

  /* Check if all conditions necessary to begin a backup are met */
  QStringList check = this->ready();

  if(!check.isEmpty()) {
    QMessageBox::critical(
      this,
      tr("Wrong parameters"),
      tr("Some backup parameters are unset or set incorrectly!\nPlease"
        " verify them before continuing.\nError report:\n") + 
        check.join('\n'),
      QMessageBox::Ok
    );
    return;
  }

  /* 
   * Prompt the user to confirm his decision to begin the backup process
   */
  if(QMessageBox::question(
    this,
    tr("Ready"),
    tr("Backup is ready to be performed.\nPlease, verify all backup"
      " parameters before you continue.\nAre you sure you want to begin"
      "the backup process?"),
    QMessageBox::Yes | QMessageBox::No
  ) == QMessageBox::Yes) {
    this->ui->buttonBackup->setEnabled(false);
    this->worker->setProgress(true);
    this->ui->buttonAbort->setEnabled(true);
    emit this->signalStart();
  }
}

/*!
 * \fn void Interface::onChooseSource(QString selected)
 * \param selected Path selected by user via directory selection dialog.
 * \brief Slot triggered when user finshes selecting backup source
 *        location.
 */
void Interface::onChooseSource(QString selected) {
  /* Update the interface */
  this->ui->progressSourceAnalysis->setMaximum(0);
  this->ui->progressSourceAnalysis->show();
  this->ui->editSourcePath->setText(selected);

  /* Launch the backup source folder contents analysis */
  this->worker->setSource(selected);
  emit this->triggerAnalysis();
}

/*!
 * \fn void Interface::onAnalysisProgress(qint64 files,
 *     qint64 directories, qint64 size)
 * \param files Current files count.
 * \param directories Current directories count.
 * \param size Total size of already processed items.
 * \brief Slot triggered by Worker each time a new item (file or
 *        directory) is discovered by backup source location
 *        analysis.
 * \see Worker
 */
void Interface::onAnalysisProgress(qint64 files, qint64 directories,
  qint64 size) {
  /* Update the interface */
  this->ui->labelDiscoveredDirectoriesValue->setText(
    QString::number(directories));
  this->ui->labelDiscoveredFilesValue->setText(QString::number(files));

  /* Convert total items size units before displaying the value */
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

  this->ui->labelDiscoveredTotalSizeValue->setText(QString::number(
    converted, 'f', 2));
}

/*!
 * \fn void Interface::onChooseTarget(QString selected)
 * \param selected Path selected by user via directory selection dialog.
 * \brief Slot triggered when user finshes selecting backup target
 *        location.
 */
void Interface::onChooseTarget(QString selected) {
  /* Update the interface */
  this->ui->editTargetPath->setText(selected);
  /* Update the Worker instance */
  this->worker->setTarget(selected);
}

/*!
 * \fn void Interface::onStatusCurrentOperation(QString operation)
 * \param operation Current operation's description.
 * \brief Slot triggered by Worker every time a new backup-related
 *        \a operation begins.
 * \see Worker
 */
void Interface::onStatusCurrentOperation(QString operation) {
  this->ui->labelStatusCurrentOperation->setText(operation);
}

/*!
 * \fn void Interface::onStatusCurrentItem(QString item)
 * \param item Currently processed item.
 * \brief Slot triggered by Worker every time a new \a item (file or
 *        directory) is being processed.
 * \see Worker
 */
void Interface::onStatusCurrentItem(QString item) {
  this->ui->labelStatusCurrentName->setText(item);
}

/*!
 * \fn void Interface::onStatusCurrentProgress(int current)
 * \param current Current item progress value in percent.
 * \brief Slot triggered by Worker every time the progress in processing
 *        of the current item (file or directory) changes.
 * \see Worker
 */
void Interface::onStatusCurrentProgress(int current) {
  this->ui->progressStatusCurrentProgress->setValue(current);
}

/*!
 * \fn void Interface::onStatusOverallProgress(int overall)
 * \param overall Overall progress value in percent.
 * \brief Slot triggered by Worker every time the \a overall progress
 *        changes.
 * \see Worker
 */
void Interface::onStatusOverallProgress(int overall) {
  if(overall < 0) {
    this->ui->progressStatusOverallProgress->setMaximum(0);
  } else {
    this->ui->progressStatusOverallProgress->setMaximum(100);
    this->ui->progressStatusOverallProgress->setValue(overall);
  }
}

/*!
 * \fn void Interface::onWorkerStarted()
 * \brief Slot triggered by Worker every time an action has begun
 *        within the latter.
 * \see Worker
 */
void Interface::onWorkerStarted() {
  this->workerInProgress = true;
  this->toggleControlsState(true);
}

/*!
 * \fn void Interface::onWorkerFinished(int action)
 * \param action Type of finished action performed within Worker.
 * \brief Slot triggered by Worker every time an \a action performed
 *        within the latter has finished.
 * \see Action
 * \see Worker
 */
void Interface::onWorkerFinished(int action) {
  this->workerInProgress = false;

  /*
   * If the action is the backup source folder contents analysis then
   * hide the corresponding progress bar
   */
  if((Action) action == ACTION_ANALYSIS) {
    this->ui->progressSourceAnalysis->setMaximum(1);
    this->ui->progressSourceAnalysis->hide();
  } else if((Action) action == ACTION_BACKUP) {
    /*
     * If the action is the backup process then reset progress information
     * and inform the user about the backup process results
     */
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
    } else if(this->worker->getErrorOccurred()) {
      QMessageBox::warning(
        this,
        tr("Backup complete with errors"),
        tr("Backup completed but errors occurred during the process!"
          " Check the log file in '%1' for more details.\n").arg(
          this->preferences->getLogsLocation()),
        QMessageBox::Ok
      );
    } else {
      QMessageBox::information(
        this,
        tr("Backup complete"),
        tr("Backup completed succefully! "),
        QMessageBox::Ok
      );
    }
  }

  this->toggleControlsState(false);
}

/*!
 * \fn void Interface::onSavePreferences()
 * \brief Slot triggered by Preferences dialog to apply the changes made
 *        by user.
 * \see Preferences
 */
void Interface::onSavePreferences() {
  /* If a backup is in progress refuse to save user changes */
  if(this->inProgress()) {
    QMessageBox::warning(
      this,
      tr("Unable to save preferences"),
      tr("A backup is currently in progress!\nThus the changes you've"
        " made to application's preferences cannot be applied!\nPlease"
        " redo your changes once the backup will be finished."),
      QMessageBox::Ok
    );
  } else {
    this->saveSettings();
  }
}

/*!
 * \fn void Interface::onDocumentationSolicitation()
 * \brief Slots that opens Backy's GitHub(R) repository in the default
 *        web browser if available.
 * 
 * If there is no default browser available an error message will be
 * displayed to user.
 */
void Interface::onDocumentationSolicitation() {
  if(!QDesktopServices::openUrl(
    QUrl("https://felsocim.github.io/Backy/"))) {
    QMessageBox::critical(
      this,
      tr("Browser unavailable"),
      tr("Unable to open the online documentation because of"
        " unavailable browser!\nPlease, check if there is a default"
        " browser set in your system and try again.\nOtherwise refer to"
        " https://felsocim.github.io/Backy/ for the documentation."),
      QMessageBox::Ok
    );
  }
}
