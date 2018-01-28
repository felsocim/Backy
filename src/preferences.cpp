#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget * parent) :
  QDialog(parent),
  ui(new Ui::Preferences),
  browseLogsLocation(new QFileDialog(this)) {
  this->ui->setupUi(this);
  this->browseLogsLocation->setFileMode(QFileDialog::Directory);
  this->setDefaults();

  QObject::connect(this->ui->buttonLogsLocationBrowse, SIGNAL(clicked(bool)), this, SLOT(onBrowseLogsLocation()));
  QObject::connect(this->browseLogsLocation, SIGNAL(fileSelected(QString)), this, SLOT(onChooseLogsLocation(QString)));
  QObject::connect(this->ui->buttonApply, SIGNAL(clicked(bool)), this, SLOT(onSave()));
  QObject::connect(this->ui->buttonDiscard, SIGNAL(clicked(bool)), this, SLOT(onDiscard()));
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
}

void Preferences::setCopyBufferSize(qint64 copyBufferSize) {
  this->copyBufferSize = copyBufferSize;
}

void Preferences::setLogsLocation(QString eventLogLocation) {
  this->logsLocation = eventLogLocation;
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
}

void Preferences::onDiscard() {
  this->setCurrents();
  this->close();
}

void Preferences::onSave() {
  this->itemBufferSize = (qint64) this->ui->spinItemBufferSize->value();
  this->copyBufferSize = (qint64) this->ui->spinCopyBufferSize->value();
  this->logsLocation = this->ui->editLogsLocationValue->text();
  emit triggerSave();
  this->close();
}
