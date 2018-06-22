QT += core gui widgets

TARGET = Backy

TEMPLATE = app

HEADERS = include/common.h \
  include/interface.h \
  include/logger.h \
  include/preferences.h \
  include/worker.h

SOURCES += src/interface.cpp \
  src/logger.cpp \
  src/preferences.cpp \
  src/worker.cpp \
  src/main.cpp

RESOURCES += res/backy.qrc

FORMS += src/interface.ui \
  src/preferences.ui

TRANSLATIONS += translate/Backy_sk.ts \
  translate/Backy_fr.ts
