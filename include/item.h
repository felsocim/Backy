#ifndef __ITEM_H
#define __ITEM_H

#include <QString>
#include <QDateTime>
#include <QFileInfo>
#include <ostream>
#include "preferences.h"

typedef enum {
  TYPE_FILE,
  TYPE_DIRECTORY
} Type;

typedef enum {
  CRITERION_MORE_RECENT,
  CRITERION_BIGGER
} Criterion;

using namespace std;

class Item {
  private:
    Type type;
    QString name,
      path;
    QDateTime lastModified;
    qint64 size;
  public:
    Item();
    Item(Type type, QString name, QString path, QDateTime lastModified, qint64 size);
    Type getType();
    QString getName();
    QString getPath();
    QDateTime getLastModified();
    qint64 getSize();
    void setType(Type type);
    void setName(QString name);
    void setPath(QString path);
    void setLastModified(QDateTime lastModified);
    void setSize(qint64 size);
    bool isSuperiorThan(QString &another, Criterion &criterion);
    friend ostream& operator<<(ostream &output, Item &source) {
      output << (source.getType() == TYPE_FILE ? "File: " : "Directory: ")
        << source.getName().toStdString()
        << ", in " << source.getPath().toStdString()
        << " was last modified on " << source.getLastModified().toString().toStdString()
        << " (" << source.getSize() << " bytes)";

      return output;
    }
};

#endif // __ITEM_H
