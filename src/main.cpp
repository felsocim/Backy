#include "../include/interface.h"
#include "../include/producer.h"
#include "../include/consumer.h"
#include "unistd.h"
#include <iostream>
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

    QSettings settings;

    settings.beginGroup("Application");
    QString locale = settings.value("locale", DEFAULT_LOCALE_CODE).toString();
    settings.endGroup();

    QTranslator translator;

    if(argc > 1) { // Arguments detected, entering CLI mode
      std::cout << QCoreApplication::applicationName().toStdString() << " " << QCoreApplication::applicationVersion().toStdString() << std::endl;
      std::cout << "(C) 2018 " << QCoreApplication::organizationName().toStdString() << std::endl;

      Producer producer;
      Consumer consumer;
      QThread pWorker, cWorker;
      std::queue<Item> buffer;
      QMutex lock;
      QWaitCondition notEmpty, notFull;
      QString source(argv[1]), destination(argv[2]);
      QDir s(source), d(destination);

      if(!s.exists()) {
        std::cerr << QCoreApplication::translate("Main", "Error: Source location does not exists").toStdString() << std::endl;
        return EXIT_FAILURE;
      }

      if(!d.exists()) {
        std::cerr << QCoreApplication::translate("Main", "Error: Destination location does not exists").toStdString() << std::endl;
        return EXIT_FAILURE;
      }

      producer->setRoot(source);

      producer->analyze();
      consumer->setDetectedCount(producer->getDirectoriesCount() + producer->getFilesCount());
      consumer->setDetectedSize(producer->getSize());

      if(producer->getDirectoriesCount() + producer->getFilesCount() < 1) {
        std::cerr << QCoreApplication::translate("Main", "Error: Source location contains no files nor folders").toStdString() << std::endl;
        goto exit;
      }

      consumer->setSource(source);
      consumer->setTarget(destination);

      int option;

      while((option = getopt(argc, argv, "sc:kb:B:l:")) != -1 ) {
        switch(option) {
          case 's':
            consumer->setSynchronize(true);
            break;
          case 'c':
            switch(optarg[0]) {
              case 'L':
                consumer->setCriterion(CRITERION_MORE_RECENT);
                break;
              case 'S':
                consumer->setCriterion(CRITERION_BIGGER);
                break;
              default:
                std::cerr << QCoreApplication::translate("Main", "Error: Unknown comparison criterion").toStdString() << std::endl;
                return EXIT_FAILURE;
            }
            break;
          case 'k':
            consumer->setKeepObsolete(true);
            break;
          case 'b':
            size_t sizeItemBuffer = QString(optarg).toULongLong();
            if(sizeItemBuffer > 1) {
              producer->setItemBufferSize(sizeItemBuffer);
            } else {
              std::cerr << QCoreApplication::translate("Main", "Error: Item buffer size set is too low (minimal value is 1 item)") << std::endl;
              return EXIT_FAILURE;
            }
            break;
          case 'B':
            qint64 sizeCopyBuffer = QString(optarg).toLongLong();
            if(sizeCopyBuffer > 1) {
              consumer->setCopyBufferSize(sizeCopyBuffer);
            } else {
              std::cerr << QCoreApplication::translate("Main", "Error: Copy buffer size set is too low (minimal value is 1 MByte)") << std::endl;
              return EXIT_FAILURE;
            }
            break;
          case 'l':
            locale = QString(optarg);
            break;
          case '?':
            std::cerr << QCoreApplication::translate("Main", "Error: Unknown option (rerun Backy with -h option to get help)") << std::endl;
            return EXIT_FAILURE;
          default:
            std::cerr << QCoreApplication::translate("Main", "Error: Incorrect sytax (rerun Backy with -h option to get help)") << std::endl;
            return EXIT_FAILURE;
            break;
        }
      }

      producer.setBuffer(&buffer);
      producer.setLock(&lock);
      producer.setNotEmpty(&notEmpty);
      producer.setNotFull(&notFull);

      consumer.setBuffer(&buffer);
      consumer.setLock(&lock);
      consumer.setNotEmpty(&notEmpty);
      consumer.setNotFull(&notFull);

      translator.load("Backy_" + locale + ".qm");
      a.installTranslator(&translator);
    } else { // No arguments provided, entering GUI mode
      translator.load("Backy_" + locale + ".qm");
      a.installTranslator(&translator);
      Interface w;
      w.show();
    }





    return a.exec();
}
