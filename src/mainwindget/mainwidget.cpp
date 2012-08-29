#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "about.h"
#include "defines.h"
#include "common.h"
#include "appsettings.h"

#include <QDesktopServices> /// for open home page
#include <QUrl>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include <QDebug>


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    init();
    createActions();
    createTrayIcon(); // add actionts to tray menu
    createConnect();
    trIcon->show();  //display tray

    //    debug();
    //    genIdx();
}
///----------------------------------------------------------------------------
MainWidget::~MainWidget()
{
    delete ui;
}
///----------------------------------------------------------------------------
void MainWidget::debug()
{
    qDebug() << "debug: mainwigdet";
    QString str = "";
    //    str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/greek.htm";
    //        str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/robinson.htm";
    //            str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/cslav.htm";
    //        str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/vikhlyantsev.htm";
    //    str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/";
    //            str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/bibleinfo.htm";
    //        str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/greek_utf-16_edit.htm";
    //    str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/BrockhausLexicon.htm";


    ui->LEFile->setText(str);

    ui->LENameDict->setText("text name1");
}
///----------------------------------------------------------------------------
void MainWidget::init()
{
    gui_about = new AboutDialog(this);

    gui_settings = new AppSettings(this);
    gui_settings->loadSettings();

    ui->LEArticles->setText("</h4>");
    ui->LEHeadLines->setText("<h4>");
    /// moved to center desktop
    QRect rect = QApplication::desktop()->availableGeometry(this);
    this->move(rect.width() / 2 - this->width() / 2,
               rect.height() / 2 - this->height() / 2);
    showHideEdit(ui->checkBEditOn->checkState());
}
///----------------------------------------------------------------------------
void MainWidget::createConnect()
{
    /// about
    connect(ui->pBAbout, SIGNAL(clicked()), SLOT(showAbout()));
    /// site
    connect(ui->pBSiteHome, SIGNAL(clicked()), SLOT(showHomeSite()));
    /// browse
    connect(ui->pBBrowseFile, SIGNAL(clicked()), SLOT(browseFile()));
    /// clear
    connect(ui->pBClear, SIGNAL(clicked()), SLOT(clearFlied()));
    /// gen idx file
    connect(ui->pBGen, SIGNAL(clicked()), SLOT(genIdx()));
    /// settings
    connect(ui->pBSettings, SIGNAL(clicked()), SLOT(showSettings()));
    /// exit
    connect(ui->pBExit, SIGNAL(clicked()), qApp, SLOT(quit()));
    /// tab edit activate
    connect(ui->checkBEditOn, SIGNAL(stateChanged(int)), SLOT(showHideEdit(int)));
}
///----------------------------------------------------------------------------
void MainWidget::createActions()
{
    minimizeAction = new QAction(tr("&Hide"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("&Expand"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("Q&uit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
}
///----------------------------------------------------------------------------
void MainWidget::createTrayIcon()
{
    trIcon = new QSystemTrayIcon();  //init
    trIcon->setIcon(QIcon(":/images/images/img.png"));  //set ico

    trayIconMenu = new QMenu(this);  // create menu
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trIcon->setContextMenu(trayIconMenu); //set menu
}
///----------------------------------------------------------------------------
void MainWidget::showHomeSite()
{
    QDesktopServices::openUrl(QUrl(GL_WEBSITE));
}
///----------------------------------------------------------------------------
void MainWidget::browseFile()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("HTML-Files (*.htm *.html);;Text files (*.txt)"));
    if (!fn.isEmpty())
    {
        ui->LEFile->setText(fn);
        //        qDebug() << "Input encoding = " << getEncodingFromFile(fn);
    }
}
///----------------------------------------------------------------------------
void MainWidget::showAbout()
{
    gui_about->show();
}
///----------------------------------------------------------------------------
void MainWidget::showHide(QSystemTrayIcon::ActivationReason r)
{
    if (r == QSystemTrayIcon::Trigger)
    {
        if ( !this->isVisible() )
        {
            this->show();
        }
        else
        {
            this->hide();
        }
    }
}
///----------------------------------------------------------------------------
void MainWidget::showSettings()
{
    gui_settings->loadSettings();
    gui_settings->show();
}
///----------------------------------------------------------------------------
void MainWidget::clearFlied()
{
    ui->LEArticles->setText("</h4>");
    ui->LEHeadLines->setText("<h4>");
    ui->LEFile->clear();
    ui->LEFind->clear();
    ui->LENameDict->clear();
    find();
}
///----------------------------------------------------------------------------
void MainWidget::find()
{

}
///----------------------------------------------------------------------------
void MainWidget::editWord()
{

}
///----------------------------------------------------------------------------
void MainWidget::genIdx()
{
    /// create output folder
    QString pathOutputFolder = QDir::currentPath() + "/Dictionaries";

    /// mkdir cur_path/Dictionaries
    QDir dir(pathOutputFolder);
    dir.mkpath(pathOutputFolder);

    /// copy file
    // надо не копировать, а перекодировать в utf-16 c bom
    QString oldPathToFile = ui->LEFile->text();
    QString fileName = getFileNameAbs(oldPathToFile) + ".htm";
    QString newPathToFile = pathOutputFolder + "/" + fileName;

    /// gen idx file
    QString fileNameIdx = pathOutputFolder + "/" +
            getFileNameAbs(oldPathToFile) + ".idx";


    /// open files
    QFile fileOld(oldPathToFile);
    if (!fileOld.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error(genIdx()): not open dict file for read: " << oldPathToFile;
        return;
    }
    /// open file for dict
    QFile fileDict(newPathToFile);
    fileDict.remove();
    if (!fileDict.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error(genIdx()): not open dict file for write: " << fileName;
        return;
    }
    /// open file for idx
    QFile fileIdx(fileNameIdx);
    fileIdx.remove();
    if (!fileIdx.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error(genIdx()): not open inx file for write: " << fileNameIdx;
        return;
    }

    /// fill idx, fill dict
    /// set codecs
    QTextStream streamInput(&fileOld);
    bool flag = false;
    QString encoding = getEncodingFromFile(oldPathToFile);
    if (encoding == "ASCII")
    {
        flag = true;
    }
    else
    {
        streamInput.setCodec(getCodecOfEncoding(encoding));
    }
    QTextStream streamIdx(&fileIdx);
    streamIdx.setCodec(getCodecOfEncoding("UTF-16"));
    streamIdx.setGenerateByteOrderMark(true);

    QTextStream streamDict(&fileDict);
    streamDict.setCodec(getCodecOfEncoding("UTF-16"));
    streamDict.setGenerateByteOrderMark(true);

    /// fill
    streamIdx << ui->LENameDict->text() << "\r\n";

    QString line, str, str2;
    int count = 0;
    count++;
    QString tagBegin = ui->LEHeadLines->text();
    QString tagEnd = ui->LEArticles->text();

    do
    {
        line = streamInput.readLine() + "\r\n";
        line.replace(tagBegin, "<h4>");
        line.replace(tagEnd, "</h4>");
        //        line.remove("<br>");
        if (line.indexOf("<h4>") == -1)
        {
            streamDict << line;
            count += line.length() * 2;
        }
    }
    while (line.indexOf("<h4>") == -1
           and !streamInput.atEnd());

    do
    {
        line.replace(tagBegin, "<h4>");
        line.replace(tagEnd, "</h4>");

        int h4 = QString("<h4>").length();

        int posH4_1 = line.indexOf("<h4>");
        int posH4_2 = line.indexOf("<h4>", posH4_1);

        while ((posH4_2 == -1 or posH4_1 == posH4_2)
               and
               !streamInput.atEnd())
        {
            line.append(streamInput.readLine() + "\r\n");
            posH4_2 = line.indexOf("<h4>", posH4_1 + h4);
        }
        str = line.mid(posH4_1,
                       posH4_2 - posH4_1);

        int posBegin = str.indexOf("<h4>");
        int posEnd = str.indexOf("</h4>");
        str2 = line.mid(posBegin + QString("<h4>").length(),
                        posEnd - posBegin - QString("<h4>").length());
        if (!str.isEmpty())
        {
            streamDict << str;
        }

        if (!str2.isEmpty())
        {
            int count2;
            if (count %2 != 0)
                count++;

            count2 = count;
            streamIdx << str2 << "\r\n" << QString::number(count2) + "\r\n";
        }
        count += (str.length()) *2;
        line.remove(str);
        line.append(streamInput.readLine() + "\r\n");
    } while (!streamInput.atEnd());
    //        QMessageBox::information(0, "Gen idx file", "Operation Complete");
}
///----------------------------------------------------------------------------
void MainWidget::showHideEdit(int flag)
{
//    ui->
    ui->LEFind->setEnabled(flag);
    ui->tableEdit->setEnabled(flag);
    ui->pBAddWord->setEnabled(flag);
    ui->label->setEnabled(flag);
    ui->label_6->setEnabled(flag);


}

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
