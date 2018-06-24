/*!
 * \headerfile preferences.h
 * \title Preferences
 * \brief The preferences.h file contains declarations related to the Preferences class.
 * \author Marek Felsoci
 */
#include "../include/preferences.h"
#include "ui_preferences.h"

/*!
 * \fn Preferences::Preferences(QWidget * parent)
 * \param parent Pointer to the parent widget.
 * \brief Constructor of the Preferences class.
 */
Preferences::Preferences(QWidget * parent) :
  QDialog(parent),
  ui(new Ui::Preferences),
  browseLogsLocation(new QFileDialog(this)) {
  this->ui->setupUi(this);

  /* Locate available translations */
  QDirIterator i(LOCALE_RELATIVE_PATH, QStringList() << "*.qm");

  this->ui->comboLanguages->addItem(QIcon(":icons/enu.png"), "English (United States)", QVariant("enu"));

  while(i.hasNext()) {
    QString current = i.next();
    QRegExp filter(QCoreApplication::applicationName() + "_([a-z]{2,3})");
    if(filter.indexIn(current) > 0) {
      QString code = filter.cap(1);
      QLocale locale(code);
      this->ui->comboLanguages->addItem(QIcon(":icons/" + code + ".png"), locale.nativeLanguageName() + " (" + locale.nativeCountryName() + ")", QVariant(code));
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

  lookForMemInfo.start("grep", QStringList() << "MemTotal:" << "/proc/meminfo");
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
#error "Unsupported operating system!"
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
  QObject::connect(this->ui->buttonLogsLocationBrowse, SIGNAL(clicked(bool)), this, SLOT(onBrowseLogsLocation()));
  QObject::connect(this->ui->buttonApply, SIGNAL(clicked(bool)), this, SLOT(onSave()));
  QObject::connect(this->ui->buttonDiscard, SIGNAL(clicked(bool)), this, SLOT(onDiscard()));

  QObject::connect(this->browseLogsLocation, SIGNAL(fileSelected(QString)), this, SLOT(onChooseLogsLocation(QString)));
}

/*!
 * \fn Preferences::~Preferences()
 * \brief Destructor of the Preferences class.
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
 * \return Returns current copy buffer size.
 */
qint64 Preferences::getCopyBufferSize() const {
  return this->copyBufferSize;
}

/*!
 * \fn QString Preferences::getLogsLocation() const
 * \return Returns current path to the log files location.
 */
QString Preferences::getLogsLocation() const {
  return this->logsLocation;
}

/*!
 * \fn QVariant Preferences::getLocale() const
 * \return Returns the code of currently used display language.
 */
QVariant Preferences::getLocale() const {
  return this->locale;
}

/*!
 * \fn void Preferences::setCopyBufferSize(qint64 copyBufferSize)
 * \param copyBufferSize New copy buffer size.
 * \brief Assigns a new value to the copy buffer size.
 */
void Preferences::setCopyBufferSize(qint64 copyBufferSize) {
  this->copyBufferSize = copyBufferSize;
  this->ui->spinCopyBufferSize->setValue((int) copyBufferSize);
}

/*!
 * \fn void Preferences::setLogsLocation(QString eventLogLocation)
 * \param eventLogLocation New path to the log files location.
 * \brief Assigns a new value to the log files location.
 */
void Preferences::setLogsLocation(QString eventLogLocation) {
  this->logsLocation = eventLogLocation;
  this->ui->editLogsLocationValue->setText(eventLogLocation);
}

/*!
 * \fn void Preferences::setLocale(QVariant locale)
 * \param locale New code of display language.
 * \brief Assigns a new value to the code of display language.
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
 * \brief Assigns default values to the Preferences::copyBufferSize and Preferences::logsLocation.
 */
void Preferences::setDefaults() {
  this->copyBufferSize = DEFAULT_COPY_BUFFER_SIZE;
  this->logsLocation = DEFAULT_LOGS_LOCATION;

  this->setCurrents();
}

/*!
 * \fn void Preferences::onBrowseLogsLocation()
 * \brief Slot triggered when the user clicks on the \b Browse... button in order to select the folder where the log files should be stored.
 */
void Preferences::onBrowseLogsLocation() {
  this->browseLogsLocation->show();
}

/*!
 * \fn void Preferences::onChooseLogsLocation(QString selected)
 * \param selected Path to the selected folder where the log files should be stored.
 * \brief Slots triggered when the user chooses a folder where the log files should be stored.
 */
void Preferences::onChooseLogsLocation(QString selected) {
  this->ui->editLogsLocationValue->setText(selected);
}

/*!
 * \fn void Preferences::onDiscard()
 * \brief Slot triggered when the user click on the \b Discard button in order to close the Preferences dialog without saving any changes.
 */
void Preferences::onDiscard() {
  this->setCurrents();
  this->close();
}

/*!
 * \fn void Preferences::onSave()
 * \brief Slot triggered when the user click on the \b Save button in order to close the Preferences dialog and save the changes he made.
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
