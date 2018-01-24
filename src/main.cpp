#include "interface.h"
#include "producer.h"
#include "consumer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    std::queue<Item> buff;
    QFileInfo root("/home/marek/Dokumenty/projekty");
    //buff.push(Item(TYPE_DIRECTORY, root.fileName(), root.filePath(), root.lastModified(), root.size()));
    QMutex lock;
    QWaitCondition notEmpty, notFull;


    Producer prod(&buff, &lock, &notEmpty, &notFull, 5, "eventP.log", "errorP.log");

    prod.setRoot(QString::fromUtf8("/home/marek/Dokumenty/projekty"));
    prod.analyze();

    if(prod.getDirectoriesCount() + prod.getFilesCount() > 0) {
      Consumer cons(&buff, &lock, &notEmpty, &notFull, 5, "eventC.log", "errorC.log");
      cons.setDetectedCount(prod.getDirectoriesCount() + prod.getFilesCount());
      cons.setSource("/home/marek/Dokumenty/projekty");
      cons.setTarget("/home/marek/test");
      cons.setTemporary("/tmp/backy.txt");
      cons.setKeepObsolete(false);
      prod.start();
      cons.start();
      prod.wait();
      cons.wait();
    }

    //Interface w;
    //w.show();

    qDebug() << "Ended";

    return 0;

    //return a.exec();
}
