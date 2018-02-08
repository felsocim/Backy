#include "../include/interface.h"
#include "../include/producer.h"
#include "../include/consumer.h"
#include <iostream>
#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Backy");
    QCoreApplication::setOrganizationName("Feldev");
    QCoreApplication::setOrganizationDomain("marekonline.eu");
    QCoreApplication::setApplicationVersion("1.0");

    QSettings settings;

    settings.beginGroup("Application");
    QString locale = settings.value("locale", DEFAULT_LOCALE_CODE).toString();
    settings.endGroup();

    QTranslator translator;

    translator.load("Backy_" + locale + ".qm");
    a.installTranslator(&translator);
    Interface w;
    w.show();

    return a.exec();
}
