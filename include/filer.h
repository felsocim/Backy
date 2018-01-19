#ifndef FILER_H
#define FILER_H

#include <QThread>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include <QFileInfo>

class Filer : public QThread
{
  Q_OBJECT
  private:
    QString root,
      target,
      currentlyProcessed;
    int directoriesToProcess,
      filesToProcess,
      alreadyProcessed;
  public:
    explicit Filer(QObject *parent = 0);
    void run();
    void computeStats();
    int getDirectoriesToProcess();
    int getFilesToProcess();
    void setRoot(QString root);
    void setTarget(QString target);
  signals:
    void currentStatus(int processed, QString current);
};

#endif // FILER_H
