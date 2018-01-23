#include "item.h"

Item::Item() {}

Item::Item(Type type, QString name, QString path, QDateTime lastModified, qint64 size) {
  this->type = type;
  this->name = name;
  this->path = path;
  this->lastModified = lastModified;
  this->size = size;
}

Type Item::getType() {
  return this->type;
}

QString Item::getName() {
  return this->name;
}

QString Item::getPath() {
  return this->path;
}

QDateTime Item::getLastModified() {
  return this->lastModified;
}

qint64 Item::getSize() {
  return this->size;
}

void Item::setType(Type type) {
  this->type = type;
}

void Item::setName(QString name) {
  this->name = name;
}

void Item::setPath(QString path) {
  this->path = path;
}

void Item::setLastModified(QDateTime lastModified) {
  this->lastModified = lastModified;
}

void Item::setSize(qint64 size) {
  this->size = size;
}

bool Item::isSuperiorThan(QString &another, Criterion &criterion) {
  QFileInfo info(another);
  switch(criterion) {
    case CRITERION_MORE_RECENT:
      return this->lastModified > info.lastModified();
    case CRITERION_BIGGER:
      return this->size > another.size();
    default:
      break;
  }
  return true;
}
