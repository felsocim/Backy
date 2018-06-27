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
 * \file preferences.h
 * \brief Contains declarations related to Preferences class.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0
 */
#include "../include/preferences.h"
#include "ui_preferences.h"

/*!
 * \fn Preferences::Preferences(QWidget * parent)
 * \param parent Pointer to parent widget.
 * \brief Constructor of Preferences class.
 */
Preferences::Preferences(QWidget * parent) :
  QDialog(parent),
  ui(new Ui::Preferences),
  browseLogsLocation(new QFileDialog(this)) {
  this->ui->setupUi(this);

  /* Locate available translations */
  QDirIterator i(LOCALE_RELATIVE_PATH, QStringList() << "*.qm");

  this->ui->comboLanguages->addItem(QIcon(":icons/enu.png"), "English"
    " (United States)", QVariant("enu"));

  while(i.hasNext()) {
    QString current = i.next();
    QRegExp filter(QCoreApplication::applicationName() +
      "_([a-z]{2,3})");
    if(filter.indexIn(current) > 0) {
      QString code = filter.cap(1);
      QLocale locale(code);
      this->ui->comboLanguages->addItem(QIcon(":icons/" + code +
       ".png"), locale.nativeLanguageName() + " (" +
       locale.nativeCountryName() + ")", QVariant(code));
    }
  }

  /* Initialize class members with their default values */
  this->browseLogsLocation->setFileMode(QFileDialog::Directory);

  this->setDefaults();

  /* Try to determine the amount of installed RAM (in megabytes) */
  QString result;
  int dwResult = 0;

#if defined Q_OS_LINUX
  QProcess lookForMemInfo;

  lookForMemInfo.start("grep", QStringList() << "MemTotal:" <<
    "/proc/meminfo");
  lookForMemInfo.waitForFinished();

  result = lookForMemInfo.readAllStandardOutput();
  QRegExp filter("(\\d+)");

  if(filter.indexIn(result) < 0) {
    goto error;
  } else {
    result = filter.cap(1);
    dwResult = (int) (result.toLongLong() / KILOBYTE);
    result = QString::number(dwResult);
    goto success;
  }
#elif defined Q_OS_WIN
  MEMORYSTATUSEX memStatus;

  memStatus.dwLength = sizeof(memStatus);

  if(GlobalMemoryStatusEx(&memStatus) == FALSE) {
    goto error;
  } else {
    dwResult = (int) (memStatus.ullTotalPhys / MEGABYTE);
    result = QString::number(dwResult);
    goto success;
  }
#else
#error Unsupported operating system!
#endif

  error:
  this->ui->lcdDetectedRAM->display("ERR");
  this->ui->spinCopyBufferSize->setMaximum(1);
  goto following;

  success:
  this->ui->lcdDetectedRAM->display(result);
  this->ui->spinCopyBufferSize->setMaximum(dwResult);

  /* Perform signal-slot connections */
  following:
  QObject::connect(this->ui->buttonLogsLocationBrowse,
    SIGNAL(clicked(bool)), this, SLOT(onBrowseLogsLocation()));
  QObject::connect(this->ui->buttonApply, SIGNAL(clicked(bool)), this,
    SLOT(onSave()));
  QObject::connect(this->ui->buttonDiscard, SIGNAL(clicked(bool)), this,
    SLOT(onDiscard()));

  QObject::connect(this->browseLogsLocation,
    SIGNAL(fileSelected(QString)), this,
    SLOT(onChooseLogsLocation(QString)));
}

/*!
 * \fn Preferences::~Preferences()
 * \brief Destructor of Preferences class.
 */
Preferences::~Preferences() {
  delete this->browseLogsLocation;
  delete this->ui;
}
/*!
 * \fn void Preferences::setCurrents()
 * \brief Updates the interface with current values.
 */
void Preferences::setCurrents() {
  this->ui->spinCopyBufferSize->setValue(this->copyBufferSize);
  this->ui->editLogsLocationValue->setText(this->logsLocation);
}

/*!
 * \fn qint64 Preferences::getCopyBufferSize() const
 * \returns Current copy buffer size.
 */
qint64 Preferences::getCopyBufferSize() const {
  return this->copyBufferSize;
}

/*!
 * \fn QString Preferences::getLogsLocation() const
 * \returns Current path to log files location.
 */
QString Preferences::getLogsLocation() const {
  return this->logsLocation;
}

/*!
 * \fn QVariant Preferences::getLocale() const
 * \returns Code of currently used display language.
 */
QVariant Preferences::getLocale() const {
  return this->locale;
}

/*!
 * \fn void Preferences::setCopyBufferSize(qint64 copyBufferSize)
 * \param copyBufferSize New copy buffer size.
 * \brief Assigns new value to copy buffer size.
 */
void Preferences::setCopyBufferSize(qint64 copyBufferSize) {
  this->copyBufferSize = copyBufferSize;
  this->ui->spinCopyBufferSize->setValue((int) copyBufferSize);
}

/*!
 * \fn void Preferences::setLogsLocation(QString eventLogLocation)
 * \param eventLogLocation New path to log files location.
 * \brief Redefines current path to log files location.
 */
void Preferences::setLogsLocation(QString eventLogLocation) {
  this->logsLocation = eventLogLocation;
  this->ui->editLogsLocationValue->setText(eventLogLocation);
}

/*!
 * \fn void Preferences::setLocale(QVariant locale)
 * \param locale New locale code.
 * \brief Replaces current code of display language by a new value.
 */
void Preferences::setLocale(QVariant locale) {
  this->locale = locale;
  int index = this->ui->comboLanguages->findData(locale);
  if(index > -1) {
    this->ui->comboLanguages->setCurrentIndex(index);
  } else {
    this->ui->comboLanguages->setCurrentIndex(0);
  }
}

/*!
 * \fn void Preferences::setDefaults()
 * \brief Assigns default values to Preferences::copyBufferSize and
 *        Preferences::logsLocation.
 */
void Preferences::setDefaults() {
  this->copyBufferSize = DEFAULT_COPY_BUFFER_SIZE;
  this->logsLocation = DEFAULT_LOGS_LOCATION;

  this->setCurrents();
}

/*!
 * \fn void Preferences::onBrowseLogsLocation()
 * \brief Slot triggered on log files location browse.
 */
void Preferences::onBrowseLogsLocation() {
  this->browseLogsLocation->show();
}

/*!
 * \fn void Preferences::onChooseLogsLocation(QString selected)
 * \param selected Path to log files location selected via file
 *        selection dialog.
 * \brief Slots triggered when a new log files location has been
 *        selected.
 */
void Preferences::onChooseLogsLocation(QString selected) {
  this->ui->editLogsLocationValue->setText(selected);
}

/*!
 * \fn void Preferences::onDiscard()
 * \brief Slot triggered change(s) discard.
 */
void Preferences::onDiscard() {
  this->setCurrents();
  this->close();
}

/*!
 * \fn void Preferences::onSave()
 * \brief Slot triggered on change(s) save.
 */
void Preferences::onSave() {
  /* Update class members with the new preferences values */
  this->copyBufferSize = (qint64) this->ui->spinCopyBufferSize->value();
  this->logsLocation = this->ui->editLogsLocationValue->text();
  this->locale = this->ui->comboLanguages->currentData();

  /* Propagate changes */
  emit triggerSave();
  this->close();
}
