/*!
 * This file is a part of Backy project, a simple backup creation and
 * maintaining solution.
 * 
 * Copyright (C) 2018 Marek Felsoci <marek.felsoci@gmail.com> (Feldev)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * \file main.cpp
 * \brief This file is the entry point of the application.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0
 */
#include "../include/interface.h"
#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>

/*!
 * \fn int main(int argc, char *argv[])
 * \param argc Count of provided argument(s).
 * \param argv Values of provided argument(s).
 * \brief Main function representing the entry point of the application.
 * \returns Return code of the application.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* Set application's information */
    QCoreApplication::setApplicationName("Backy");
    QCoreApplication::setOrganizationName("Feldev");
    QCoreApplication::setOrganizationDomain(
      "https://github.com/felsocim");
    QCoreApplication::setApplicationVersion("1.0");

    /* Try to load preferred display language of the application */
    QSettings settings;

    settings.beginGroup("Application");

    QString locale = settings.value("locale",
      DEFAULT_LOCALE_CODE).toString();

    settings.endGroup();

    QTranslator translator;

    /* Load translation file */
    translator.load(QString(LOCALE_RELATIVE_PATH) + "/" +
      QCoreApplication::applicationName() + "_" + locale + ".qm");
    a.installTranslator(&translator);

    /* Show graphical user interface */
    Interface w;
    w.show();

    return a.exec();
}
