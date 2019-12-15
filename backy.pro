QT += core gui widgets

unix:!macx {
  QMAKE_CXXFLAGS += $$QMAKE_CFLAGS -std=c++11
}

TARGET = backy

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

RC_FILE = res/backy.rc

FORMS += src/interface.ui \
  src/preferences.ui

TRANSLATIONS += translate/backy_sk.ts \
  translate/backy_fr.ts

unix:!macx {
  program.files = backy
  program.path = $$(DESTDIR)/bin

  language.files = translate/backy_sk.qm \
                   translate/backy_fr.qm
  language.path = $$(DESTDIR)/bin/translations

  icon.files = res/backy.png
  icon.path = $$(DESTDIR)/usr/share/pixmaps

  launcher.files = res/backy.desktop
  launcher.path = $$(DESTDIR)/usr/share/applications

  INSTALLS += program \
              language \
              icon \
              launcher
}
