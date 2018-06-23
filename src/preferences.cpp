#include "../include/preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget * parent) :
  QDialog(parent),
  ui(new Ui::Preferences),
  browseLogsLocation(new QFileDialog(this)) {
  this->ui->setupUi(this);

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

  this->browseLogsLocation->setFileMode(QFileDialog::Directory);

  this->setDefaults();

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

  following:
  QObject::connect(this->ui->buttonLogsLocationBrowse, SIGNAL(clicked(bool)), this, SLOT(onBrowseLogsLocation()));
  QObject::connect(this->ui->buttonApply, SIGNAL(clicked(bool)), this, SLOT(onSave()));
  QObject::connect(this->ui->buttonDiscard, SIGNAL(clicked(bool)), this, SLOT(onDiscard()));

  QObject::connect(this->browseLogsLocation, SIGNAL(fileSelected(QString)), this, SLOT(onChooseLogsLocation(QString)));
}

Preferences::~Preferences() {
  delete this->browseLogsLocation;
  delete this->ui;
}

void Preferences::setCurrents() {
  this->ui->spinCopyBufferSize->setValue(this->copyBufferSize);
  this->ui->editLogsLocationValue->setText(this->logsLocation);
}

qint64 Preferences::getCopyBufferSize() const {
  return this->copyBufferSize;
}

QString Preferences::getLogsLocation() const {
  return this->logsLocation;
}

QVariant Preferences::getLocale() const {
  return this->locale;
}

void Preferences::setCopyBufferSize(qint64 copyBufferSize) {
  this->copyBufferSize = copyBufferSize;
  this->ui->spinCopyBufferSize->setValue((int) copyBufferSize);
}

void Preferences::setLogsLocation(QString eventLogLocation) {
  this->logsLocation = eventLogLocation;
  this->ui->editLogsLocationValue->setText(eventLogLocation);
}

void Preferences::setLocale(QVariant locale) {
  this->locale = locale;
  int index = this->ui->comboLanguages->findData(locale);
  if(index > -1) {
    this->ui->comboLanguages->setCurrentIndex(index);
  } else {
    this->ui->comboLanguages->setCurrentIndex(0);
  }
}

void Preferences::setDefaults() {
  this->copyBufferSize = DEFAULT_COPY_BUFFER_SIZE;
  this->logsLocation = DEFAULT_LOGS_LOCATION;

  this->setCurrents();
}

void Preferences::onBrowseLogsLocation() {
  this->browseLogsLocation->show();
}

void Preferences::onChooseLogsLocation(QString selected) {
  this->ui->editLogsLocationValue->setText(selected);
}

void Preferences::onDiscard() {
  this->setCurrents();
  this->close();
}

void Preferences::onSave() {
  this->copyBufferSize = (qint64) this->ui->spinCopyBufferSize->value();
  this->logsLocation = this->ui->editLogsLocationValue->text();
  this->locale = this->ui->comboLanguages->currentData();

  emit triggerSave();
  this->close();
}
