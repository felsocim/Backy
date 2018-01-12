#ifndef ITEM_H
#define ITEM_H

#include <QString>

typedef enum {
  ITEM_FILE,
  ITEM_DIRECTORY
} ItemType;

class Item {
  private:
    QString name;
    ItemType type;
  public:
    Item();
    Item(QString name, ItemType type);
    QString getName();
    ItemType getType();
    void setName(QString name);
    void setType(ItemType type);
};

#endif // ITEM_H
