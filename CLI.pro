QT += core widgets

CONFIG += console

TARGET = Backy

HEADERS = include/common.h \
  include/consumer.h \
  include/item.h \
  include/logger.h \
  include/producer.h \
  include/worker.h \
  lib/parseopt/include/option.h

SOURCES += src/consumer.cpp \
  src/item.cpp \
  src/logger.cpp \
  src/producer.cpp \
  src/worker.cpp \
  src/cli.cpp \
  lib/parseopt/src/option.cpp

TRANSLATIONS += translate/Backy_sk.ts \
  translate/Backy_fr.ts
