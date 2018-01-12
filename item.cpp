#include "item.h"

Item::Item() {}

Item::Item(QString name, ItemType type) {
  this->name = name;
  this->type = type;
}

QString Item::getName() {
  return this->name;
}

ItemType Item::getType() {
  return this->type;
}

void Item::setName(QString name) {
  this->name = name;
}

void Item::setType(ItemType type) {
  this->type = type;
}
