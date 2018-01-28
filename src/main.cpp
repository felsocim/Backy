#include "../include/interface.h"
#include "../include/producer.h"
#include "../include/consumer.h"
#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Backy");
    QCoreApplication::setOrganizationName("Marek Felsoci");
    QCoreApplication::setOrganizationDomain("marekonline.eu");
    QCoreApplication::setApplicationVersion("1.0");

    QTranslator translator;
    translator.load("Backy_sk.qm");
    a.installTranslator(&translator);

    Interface w;
    w.show();

    return a.exec();
}
