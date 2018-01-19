#ifndef __ITEM_H
#define __ITEM_H

#include <string>

typedef enum {
  TYPE_FILE,
  TYPE_DIRECTORY
} Type;

using namespace std;

class Item {
  private:
    Type type;
    string name,
      path;
    time_t lastModified;
    off_t size;
  public:
    Item();
    Item(Type type, string name, string path, time_t lastModified, off_t size);
    ~Item();
    Type getType();
    string getName();
    string getPath();
    time_t getLastModified();
    off_t getSize();
    void setType(Type type);
    void setName(string name);
    void setPath(string path);
    void setLastModified(time_t lastModified);
    void setSize(off_t size);
};

#endif // __ITEM_H
