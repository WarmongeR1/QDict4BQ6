#include <QtGui/QApplication>
#include "mainwidget.h"
#include "defines.h"
#include <QSettings>
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{

//    Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);

    a.setOrganizationName(GL_AUTHOR);
    a.setApplicationName(GL_PROJECT_NAME);

    QSettings *settings = new QSettings("settings.conf",QSettings::NativeFormat);
    QString lang = settings->value("language/lang").toString();

    QTranslator translator;
    if (lang == "Russian" or lang.isEmpty()) translator.load("QDict4BQ6_ru",":lang/lang");
    if (lang == "Deutch") translator.load("QDict4BQ6_de",":lang/lang");
    if (lang == "France") translator.load("QDict4BQ6_fr",":lang/lang");
    a.installTranslator(&translator);

    MainWidget w;
    w.show();

    return a.exec();
}
