#ifndef FILER_H
#define FILER_H

#include <QString>
#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>

class Filer
{
  private:
    QString root;
    int directoriesToProcess,
      filesToProcess,
      currentlyProcessing;
  public:
    Filer();
    Filer(QString root);
};

#endif // FILER_H
