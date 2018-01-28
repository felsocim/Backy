#include "item.h"

Item::Item() {}

Item::Item(const Type &type, const QString &name, const QString &path, const QDateTime &lastModified, const qint64 &size) {
  this->type = type;
  this->name = name;
  this->path = path;
  this->lastModified = lastModified;
  this->size = size;
}

Type Item::getType() const {
  return this->type;
}

QString Item::getName() const {
  return this->name;
}

QString Item::getPath() const {
  return this->path;
}

QDateTime Item::getLastModified() const {
  return this->lastModified;
}

qint64 Item::getSize() const {
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

bool Item::isSuperiorThan(const QString &another, const Criterion &criterion) {
  QFileInfo info(another);

  switch(criterion) {
    case CRITERION_MORE_RECENT:
      return this->lastModified.toSecsSinceEpoch() > info.lastModified().toSecsSinceEpoch();
    case CRITERION_BIGGER:
      return this->size > another.size();
    default:
      break;
  }

  return true;
}
