#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget * parent) :
  QDialog(parent),
  ui(new Ui::Preferences),
  browseLogsLocation(new QFileDialog(this)) {
  this->ui->setupUi(this);

  this->browseLogsLocation->setFileMode(QFileDialog::Directory);

  this->logsLocationChanged = false;

  this->setDefaults();

#if defined Q_OS_LINUX
  QProcess lookForMemInfo;

  lookForMemInfo.start("grep", QStringList() << "MemTotal:" << "/proc/meminfo");
  lookForMemInfo.waitForFinished();

  QString result = lookForMemInfo.readAllStandardOutput();
  QRegExp filter("(\\d+)");

  if(filter.indexIn(result) < 0) {
    this->ui->lcdDetectedRAM->display("ERR");
    this->ui->spinCopyBufferSize->setMaximum(1);
  } else {
    int result = (int) (filter.cap(1).toLongLong() / KILOBYTE);
    this->ui->lcdDetectedRAM->display(result);
    this->ui->spinCopyBufferSize->setMaximum(result);
  }
#else
#error "Unsupported operating system!"
#endif

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
  this->ui->spinItemBufferSize->setValue(this->itemBufferSize);
  this->ui->spinCopyBufferSize->setValue(this->copyBufferSize);
  this->ui->editLogsLocationValue->setText(this->logsLocation);
}

qint64 Preferences::getItemBufferSize() const {
  return this->itemBufferSize;
}

qint64 Preferences::getCopyBufferSize() const {
  return this->copyBufferSize;
}

QString Preferences::getLogsLocation() const {
  return this->logsLocation;
}

void Preferences::setItemBufferSize(qint64 itemBufferSize) {
  this->itemBufferSize = itemBufferSize;
  this->ui->spinItemBufferSize->setValue((int) itemBufferSize);
}

void Preferences::setCopyBufferSize(qint64 copyBufferSize) {
  this->copyBufferSize = copyBufferSize;
  this->ui->spinCopyBufferSize->setValue((int) copyBufferSize);
}

void Preferences::setLogsLocation(QString eventLogLocation) {
  this->logsLocation = eventLogLocation;
  this->ui->editLogsLocationValue->setText(eventLogLocation);
}

void Preferences::setDefaults() {
  this->itemBufferSize = DEFAULT_ITEM_BUFFER_SIZE;
  this->copyBufferSize = DEFAULT_COPY_BUFFER_SIZE;
  this->logsLocation = DEFAULT_LOGS_LOCATION;

  this->setCurrents();
}

void Preferences::onBrowseLogsLocation() {
  this->browseLogsLocation->show();
}

void Preferences::onChooseLogsLocation(QString selected) {
  this->ui->editLogsLocationValue->setText(selected);
  this->logsLocationChanged = true;
}

void Preferences::onDiscard() {
  this->setCurrents();
  this->close();
}

void Preferences::onSave() {
  this->itemBufferSize = (qint64) this->ui->spinItemBufferSize->value();
  this->copyBufferSize = (qint64) this->ui->spinCopyBufferSize->value();
  this->logsLocation = this->ui->editLogsLocationValue->text();

  if(this->logsLocationChanged) {
    if(
      QMessageBox::information(
        this,
        tr("Restart required"),
        tr("Some changes you've made won't take effect until next application launch."),
        QMessageBox::Ok | QMessageBox::Cancel
      ) == QMessageBox::Ok
    ) {
      goto trigger;
    } else {
      return;
    }
  }

  trigger:
  emit triggerSave();
  this->close();
}
