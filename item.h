#ifndef ITEM_H
#define ITEM_H

#include <iostream>

typedef enum {
  ITEM_FILE,
  ITEM_DIRECTORY
} ItemType;

class Item {
  private:
    std::string name;
    ItemType type;
  public:
    Item();
    Item(std::string name, ItemType type);
    std::string getName();
    ItemType getType();
    void setName(std::string name);
    void setType(ItemType type);
};

#endif // ITEM_H
