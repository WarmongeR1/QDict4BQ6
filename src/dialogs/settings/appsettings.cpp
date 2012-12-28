#include "appsettings.h"
#include "ui_appsettings.h"
#include "defines.h"
#include <QDebug>
#include <QMessageBox>
AppSettings::AppSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppSettings)
{
    ui->setupUi(this);
    init();
    createConnect();
}
//------------------------------------------------------------------------------
AppSettings::~AppSettings()
{
    delete ui;
}
//------------------------------------------------------------------------------
void AppSettings::debug()
{

    qDebug() << "debug: appwsettings";
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
void AppSettings::loadSettings()
{
    QString lang = settings->value("language/lang").toString();
    //    qDebug() << settings->value("language/lang");
    if (lang.isEmpty())
        lang = "Russian";
    ui->comBLanguage->setCurrentIndex(ui->comBLanguage->findText(lang));
}
//------------------------------------------------------------------------------
void AppSettings::saveSettings()
{
    QString lang = ui->comBLanguage->currentText();
    settings->setValue("language/lang", lang);
    settings->sync();
}
//------------------------------------------------------------------------------
void AppSettings::createConnect()
{

}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void AppSettings::accept()
{
    QString lang = ui->comBLanguage->currentText();
    QString old_lang = settings->value("language/lang").toString();
    qDebug() << lang << old_lang;
    if (lang != old_lang)
    {
        int ret = QMessageBox::warning(this, tr(GL_PROJECT_NAME),
                                       tr("Settings has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Save
                                       | QMessageBox::Discard
                                       | QMessageBox::Cancel,
                                       QMessageBox::Save);
        QMessageBox msgBox;
        switch (ret)
        {
        case QMessageBox::Save:
            // Save was clicked
            saveSettings();

            msgBox.setText("Settings has been modified. Please restart the"
                           "application for the entry into force of the settings");
            msgBox.exec();

            QWidget::hide();
            break;
        case QMessageBox::Discard:
            //            setParams();
            QWidget::hide();
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
        default:
            // should never be reached
            break;
        }
    }
}
//------------------------------------------------------------------------------
