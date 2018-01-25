#include "interface.h"
#include "producer.h"
#include "consumer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Interface w;
    w.show();

    return a.exec();
}
