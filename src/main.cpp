/*!
 * \headerfile main.cpp
 * \title Main file
 * \brief The main.cpp file is the entry point of the application.
 */
#include "../include/interface.h"
#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>

/*!
 * \fn int main(int argc, char *argv[])
 * \brief Main function representing the entry point of the application providing the number of given arguments via the \a argc argument and the values of given arguments via the \a argv argument. The return code of this function corresponds to the return code of the application.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* Set application's information */
    QCoreApplication::setApplicationName("Backy");
    QCoreApplication::setOrganizationName("Feldev");
    QCoreApplication::setOrganizationDomain("marekonline.eu");
    QCoreApplication::setApplicationVersion("1.0");

    /* Try to load the preferred display language of the application */
    QSettings settings;

    settings.beginGroup("Application");

    QString locale = settings.value("locale", DEFAULT_LOCALE_CODE).toString();

    settings.endGroup();

    QTranslator translator;

    translator.load(QString(LOCALE_RELATIVE_PATH) + "/" + QCoreApplication::applicationName() + "_" + locale + ".qm");
    a.installTranslator(&translator);

    /* Show graphical user interface */
    Interface w;
    w.show();

    return a.exec();
}
