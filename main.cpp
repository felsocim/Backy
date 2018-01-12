#include "interface.h"
#include "reader.h"
#include "writer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMutex lock;
    QWaitCondition notEmpty;
    QWaitCondition notFull;
    std::queue<Item> buf;
    bool ended = false;
    QString init = "D:/Video";

    Reader rdr(QFile::decodeName(init.toLocal8Bit()), &buf, 5, &lock, &notEmpty, &notFull, &ended);
    Writer wrtr(&buf, &lock, &notEmpty, &notFull, &ended);

    rdr.start();
    wrtr.start();
    rdr.wait();
    wrtr.wait();

    Interface w;
    w.show();

    return a.exec();
}
