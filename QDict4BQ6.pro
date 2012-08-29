#-------------------------------------------------
#
# Project created by QtCreator 2012-08-28T11:11:13
#
#-------------------------------------------------

QT       += core gui network webkit

TARGET = Dict4BQ6
TEMPLATE = app
LANGUAGE = C++

PROJECTNAME = Dict4BQ6
DESTDIR     = build/bin
OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
RCC_DIR     = build/rc
UI_DIR      = build/ui



DEPENDPATH += \
    src/dialogs/about \
    src/dialogs \
    src/main \
    src/mainwindget \
    src/common \
    src/defines \
    src/textedit \
    src/dialogs/settings \
    src/htmleditor \
    src/highlighter

INCLUDEPATH += \
    src/dialogs/about \
    src/main \
    src/defines \
    src/mainwindget \
    src/common \
    src/textedit \
    src/dialogs/settings \
    src/htmleditor \
    src/highlighter

SOURCES += main.cpp\
        mainwidget.cpp \
        about.cpp \
        common.cpp \
        appsettings.cpp \
        htmleditor.cpp \
        highlighter.cpp

HEADERS  += mainwidget.h \
        about.h \
        defines.h\
        common.h \
        appsettings.h \
        htmleditor.h \
        highlighter.h

FORMS    += mainwidget.ui \
        about.ui \
        appsettings.ui \
        htmleditor.ui \
        inserthtmldialog.ui

RESOURCES += \
    resources/resources.qrc \
    resources/resources.qrc

TRANSLATIONS += \
    resources/lang/QDict4BQ6_ru.ts
