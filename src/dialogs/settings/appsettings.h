#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
    class AppSettings;
}


class AppSettings : public QDialog
{
    Q_OBJECT

public:
    explicit AppSettings(QWidget *parent = 0);
    ~AppSettings();

    /**
      @function
      Init
      */
    void init();
    /**
      @function
      Load settings
      */
    void loadSettings();
    /**
      @function
      Save settings
      */
    void saveSettings();
private slots:
    void accept();

private:
    Ui::AppSettings *ui;
    QSettings *settings;

    void debug();

    /**
      @function
      Create connects
      */
    void createConnect();

};

#endif // APPSETTINGS_H
