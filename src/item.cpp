#include "item.h"

Item::Item() {}

Item::Item(Type type, string name, string path, time_t lastModified, off_t size) {
  this->type = type;
  this->name = name;
  this->path = path;
  this->lastModified = lastModified;
  this->size = size;
}

Item::~Item() {}

Type Item::getType() {
  return this->type;
}

string Item::getName() {
  return this->name;
}

string Item::getPath() {
  return this->path;
}

time_t Item::getLastModified() {
  return this->lastModified;
}

off_t Item::getSize() {
  return this->size;
}

void Item::setType(Type type) {
  this->type = type;
}

void Item::setName(string name) {
  this->name = name;
}

void Item::setPath(string path) {
  this->path = path;
}

void Item::setLastModified(time_t lastModified) {
  this->lastModified = lastModified;
}

void Item::setSize(off_t size) {
  this->size = size;
}
