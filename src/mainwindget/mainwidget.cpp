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
#include <QStringList>
#include <QDate>

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

        debug();
        genIdx();
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
//                str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/cslav.htm";
    //        str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/vikhlyantsev.htm";
    //    str = "/home/files/Develop/git/projectB/projectB/bin/example";
    //            str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/bibleinfo.htm";
    //        str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/greek_utf-16_edit.htm";
    //    str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/BrockhausLexicon.htm";
    str = "/home/files/Develop/git/projectB/projectB/bin/example/cslav.htm";

    ui->LEFile->setText(str);

    ui->LENameDict->setText("text name1");
    ui->LEAuthor->setText("test");
//    getParams();
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
    // TODO: edit
    QString encoding = getEncodingFromFile(oldPathToFile);
    if (encoding == "ASCII")
    {
        flag = true;
    }
    else
    {
        streamInput.setCodec(getCodecOfEncoding(encoding));
    }
    // TODO: end edit

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

    //----------
    /// skip begin text

    do
    {
        line = streamInput.readLine() + "\r\n";

        /// edit to standart
        /// replace tag to standart
        line.replace(tagBegin, "<h4>");
        line.replace(tagEnd, "</h4>");

        if (line.indexOf("<h4>") == -1)
        {

//            if (line.indexOf("<title>") != -1)



            streamDict << line;
            count += line.length() * 2;
        }
    }
    while (line.indexOf("<h4>") == -1
           and !streamInput.atEnd());

    if (ui->checkBParams->checkState())
    {
        QStringList list = getParams();


        qDebug() << QDate::currentDate().toString();

        streamDict << "<html>\r\n";
        streamDict << "<head>\r\n";
        streamDict << "\t<meta name=\"Author\" content=\"" << list.at(0) << "\">\r\n";
        streamDict << "\t<meta name=\"Date\" content=\"DateOfDict\">\r\n";
        streamDict << "\t<meta name=\"Date\" content=\"" <<
                      QDate::currentDate().toString() << "\">\r\n";
        streamDict << "\t<meta name=\"Revision\" content=\"" << list.at(1) << "\">\r\n";
        streamDict << "\t<meta name=\"Language\" content=\"" << list.at(2) <<  "\">\r\n";
        streamDict << "\t<meta name=\"Type\" content=\"" << list.at(3) << "\">\r\n";
        streamDict << "\t<meta name=\"Description\" content=\"" << list.at(4) << "\">\r\n";
        streamDict << "\t<meta name=\"Rights\" content=\"" << list.at(5) << "\">\r\n";
        streamDict << "\t<meta name=\"Numbering\" content=\"" << list.at(6) << "\">\r\n";
        streamDict << "<title>" << ui->LENameDict->text() << "</title>\r\n";
        streamDict << "</head>\r\n";
    }

    //-----------

    do
    {
        /// edit to standart
        /// replace tag to standart
        line.replace(tagBegin, "<h4>");
        line.replace(tagEnd, "</h4>");

        int h4 = QString("<h4>").length();

        int posH4_1 = line.indexOf("<h4>");
        int posH4_2 = line.indexOf("<h4>", posH4_1);

        /// split text  mid <h4> and <h4>
        /// if the text in multiple lines
        while ((posH4_2 == -1 or posH4_1 == posH4_2)
               and !streamInput.atEnd())
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
    ui->LEFind->setEnabled(flag);
    ui->tableEdit->setEnabled(flag);
    ui->pBAddWord->setEnabled(flag);
    ui->label->setEnabled(flag);
    ui->label_6->setEnabled(flag);
    if (flag)
        showWordInTable();
}
///----------------------------------------------------------------------------
void MainWidget::showWordInTable()
{
    qDebug() << "show =)";
//    for (int i = 0; i < data.files.size(); ++i)
//    {
//        QTableWidgetItem *fileNameItem = new QTableWidgetItem(data.files[i]);
//        fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);

//        QTableWidgetItem *bookItem = new QTableWidgetItem(data.books[i]);
//        bookItem->setFlags(bookItem->flags() ^ Qt::ItemIsEditable);

//        QTableWidgetItem *moduleItem = new QTableWidgetItem(data.modules[i]);
//        moduleItem->setFlags(moduleItem->flags() ^ Qt::ItemIsEditable);

//        QTableWidgetItem *verseItem = new QTableWidgetItem(data.verse[i]);
//        verseItem->setFlags(verseItem->flags() ^ Qt::ItemIsEditable);

//        QTableWidgetItem *chapterItem = new QTableWidgetItem(data.chapter[i]);
//        chapterItem->setFlags(chapterItem->flags() ^ Qt::ItemIsEditable);

//        QTableWidgetItem *urlItem = new QTableWidgetItem(data.url[i]);
//        urlItem->setFlags(urlItem->flags() ^ Qt::ItemIsEditable);

//        int row = ui->tableFiles->rowCount();
//        ui->tableFiles->insertRow(row);

//        ui->tableFiles->setItem(row, 0, moduleItem);
//        ui->tableFiles->setItem(row, 1, bookItem);
//        ui->tableFiles->setItem(row, 2, chapterItem);
//        ui->tableFiles->setItem(row, 3, verseItem);
//        ui->tableFiles->setItem(row, 4, fileNameItem);
//        ui->tableFiles->setItem(row, 5, urlItem);
//    }
//    ui->tableFiles->resizeColumnsToContents();

//    ui->filesFoundLabel->setText(tr("%1 file(s) found").arg(data.files.size()) +
//                                 (" (Double click on a file to open it)"));
}
///----------------------------------------------------------------------------
QStringList MainWidget::getParams()
{
    QStringList list;
    list << ui->LEAuthor->text(); // [0]
    list << QString::number(ui->spinBoxVersion->value()); // [1]
    list << ui->comBoxLang->currentText(); // [2]
    list << ui->comBoxType->currentText(); // [3]
    list << ui->LEDescription->text(); // [4]
    list << ui->LECopyright->text(); // [5]
    list << ui->LENumbering->text(); // [6]
    return list;
}


///----------------------------------------------------------------------------


///----------------------------------------------------------------------------

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
///----------------------------------------------------------------------------
