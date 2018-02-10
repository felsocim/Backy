QT += core gui widgets

TARGET = BackyGUI

TEMPLATE = app

HEADERS = include/common.h \
  include/consumer.h \
  include/interface.h \
  include/item.h \
  include/logger.h \
  include/preferences.h \
  include/producer.h \
  include/worker.h

SOURCES += src/consumer.cpp \
  src/interface.cpp \
  src/item.cpp \
  src/logger.cpp \
  src/preferences.cpp \
  src/producer.cpp \
  src/worker.cpp \
  src/gui.cpp

RESOURCES += res/backy.qrc

FORMS += src/interface.ui \
  src/preferences.ui

TRANSLATIONS += translate/Backy_sk.ts \
  translate/Backy_fr.ts
