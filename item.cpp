#include "item.h"

Item::Item() {}

Item::Item(std::string name, ItemType type) {
  this->name = name;
  this->type = type;
}

std::string Item::getName() {
  return this->name;
}

ItemType Item::getType() {
  return this->type;
}

void Item::setName(std::string name) {
  this->name = name;
}

void Item::setType(ItemType type) {
  this->type = type;
}
