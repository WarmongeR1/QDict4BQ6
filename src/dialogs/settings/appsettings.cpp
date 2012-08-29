#include "appsettings.h"
#include "ui_appsettings.h"

#include <QDebug>
AppSettings::AppSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppSettings)
{
    ui->setupUi(this);
    init();
    createConnect();
}
///----------------------------------------------------------------------------
AppSettings::~AppSettings()
{
    delete ui;
}
///----------------------------------------------------------------------------
void AppSettings::debug()
{

    qDebug() << "debug: appwsettings";
}
///----------------------------------------------------------------------------
void AppSettings::init()
{
    /// add item to combobox
    ui->comBLanguage->addItem("Russian");
    ui->comBLanguage->addItem("English");

    /// create QSettings
    settings = new QSettings("settings.conf",QSettings::NativeFormat);
//    QString value = "Russian";
//    settings->setValue("language/lang", value);
    settings->sync();
}
///----------------------------------------------------------------------------
void AppSettings::loadSettings()
{
    QString lang = settings->value("language/lang").toString();
//    qDebug() << settings->value("language/lang");
    ui->comBLanguage->setCurrentIndex(ui->comBLanguage->findText(lang));
}
///----------------------------------------------------------------------------
void AppSettings::saveSettings()
{
    QString lang = ui->comBLanguage->currentText();
    settings->setValue("language/lang", lang);
    settings->sync();
}
///----------------------------------------------------------------------------
void AppSettings::createConnect()
{

}
///----------------------------------------------------------------------------

///----------------------------------------------------------------------------
void AppSettings::accept()
{
    saveSettings();
    QWidget::hide();
}
///----------------------------------------------------------------------------
