QT += core gui widgets

unix:!macx {
  QMAKE_CXXFLAGS += $$QMAKE_CFLAGS -std=c++11
}

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

RC_FILE = res/backy.rc

FORMS += src/interface.ui \
  src/preferences.ui

TRANSLATIONS += translate/Backy_sk.ts \
  translate/Backy_fr.ts

unix:!macx {
  program.files = Backy
  program.path = $$(DESTDIR)/usr/bin/Backy

  language.files = translate/Backy_sk.qm \
                   translate/Backy_fr.qm
  language.path = $$(DESTDIR)/usr/bin/Backy/translations

  icon.files = res/Backy.png
  icon.path = $$(DESTDIR)/usr/share/pixmaps

  launcher.files = res/Backy.desktop
  launcher.path = $$(DESTDIR)/usr/share/applications

  INSTALLS += program \
              language \
              icon \
              launcher
}
