#ifndef __ITEM_H
#define __ITEM_H

#include <QDateTime>
#include <QFileInfo>
#include <ostream>
#include "common.h"

class Item {
  private:
    Type type;
    QString name,
      path;
    QDateTime lastModified;
    qint64 size;

  public:
    Item();
    Item(const Type &type, const QString &name, const QString &path, const QDateTime &lastModified, const qint64 &size);
    Type getType() const;
    QString getName() const;
    QString getPath() const;
    QDateTime getLastModified() const;
    qint64 getSize() const;
    void setType(Type type);
    void setName(QString name);
    void setPath(QString path);
    void setLastModified(QDateTime lastModified);
    void setSize(qint64 size);
    bool isSuperiorThan(const QString &another, const Criterion &criterion);
    friend std::ostream& operator<<(std::ostream &output, Item &source) {
      output << (source.getType() == TYPE_FILE ? "File: " : "Directory: ")
        << source.getName().toStdString()
        << ", in " << source.getPath().toStdString()
        << " was last modified on " << source.getLastModified().toString().toStdString()
        << " (" << source.getSize() << " bytes)";

      return output;
    }
};

#endif // __ITEM_H
