#-------------------------------------------------
#
# Project created by QtCreator 2012-08-28T11:11:13
#
#-------------------------------------------------

QT       += core gui

TARGET = projectB
TEMPLATE = app
LANGUAGE = C++

PROJECTNAME = projectB
DESTDIR     = build/bin
OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
RCC_DIR     = build/rc
UI_DIR      = build/ui



DEPENDPATH += \
    src/dialogs/about \
    src/main \
    src/mainwindget \
    src/common \
    src/defines \
    src/textedit \
    src/dialogs/settings

INCLUDEPATH += \
    src/dialogs/about \
    src/main \
    src/defines \
    src/mainwindget \
    src/common \
    src/textedit \
    src/dialogs/settings

SOURCES += main.cpp\
        mainwidget.cpp \
        about.cpp \
        common.cpp \
    src/dialogs/settings/appsettings.cpp

HEADERS  += mainwidget.h \
        about.h \
        defines.h\
        common.h \
    src/dialogs/settings/appsettings.h

FORMS    += mainwidget.ui \
        about.ui \
    src/dialogs/settings/appsettings.ui

RESOURCES += \
    resources/resources.qrc \
    resources/resources.qrc

TRANSLATIONS += \
    resources/lang/projectB_ru.ts
