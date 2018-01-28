#include "interface.h"
#include "producer.h"
#include "consumer.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Backy");
    QCoreApplication::setOrganizationName("Marek Felsoci");
    QCoreApplication::setOrganizationDomain("marekonline.eu");
    QCoreApplication::setApplicationVersion("1.0");

    Interface w;
    w.show();

    return a.exec();
}
