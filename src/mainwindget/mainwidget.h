#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QStringList>
#include <QStringListModel>

#include <QDate>

namespace Ui {
    class MainWidget;
}
class AboutDialog;
class AppSettings;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    /**
      @function
      Hide UI
      */
    void showHide(QSystemTrayIcon::ActivationReason);

    /**
      @function
      Show about dialog
      */
    void showAbout();

    /**
      @fucntion
      Show home page prog.
      Used QDesktopServise
      */
    void showHomeSite();

    /**
      @function
      Browse dict file
      */
    void browseFile();

    /**
      @function
      Clear flied
      */
    void clearFlied();

    /**
      @function
      Show settings
      */
    void showSettings();

    /**
      @function
      Gen idx file
      */
    void genIdx();

    /**
      @function
      Find in table
      */
    void find();

    /**
      @function
      Edit word
      */
    void editWord();

    /**
      @fucntion
      Activate or unactivate tab edit
      @param check_state  (int)
      */
    void showHideEdit(int);

private:
    Ui::MainWidget *ui;
    AboutDialog *gui_about; // about dialog
    AppSettings *gui_settings; // settings dialog

    //tray
    QSystemTrayIcon *trIcon;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    /**
      @function
      Create connects
      */
    void createConnect();
    /**
      @function
      Create actions for tray
      */
    void createActions();
    /**
      @function
      Init
      */
    void init();
    /**
      @function
      Create tray
      */
    void createTrayIcon(); // add actionts to tray menu

    void debug();
    /**
      @function
      Fill table of word in dict
      */
    void showWordInTable();

    /**
      @function
      Return info in params tab
      */
    QStringList getParams();

    /**
      @function
      Set data to lang combobox
      */
    void setLangList();

    /**
      @function
      Set data to type combobox
      */
    void setTypeList();
};

#endif // MAINWIDGET_H
