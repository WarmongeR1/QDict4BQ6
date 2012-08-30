#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "about.h"
#include "htmleditor.h"
#include "defines.h"
#include "common.h"
#include "appsettings.h"

#include <QDesktopServices> /// for open home page
#include <QUrl>
#include <QDesktopWidget>


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
    //                str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/cslav.htm";
    //        str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/vikhlyantsev.htm";
    //    str = "/home/files/Develop/git/projectB/projectB/bin/example";
    //            str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/bibleinfo.htm";
    //        str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/greek_utf-16_edit.htm";
    //    str = "/home/files/Develop/C++/Qt/MyProgs/projectB/projectB/bin/example/BrockhausLexicon.htm";
    //    str = "/home/files/Develop/git/projectB/projectB/bin/example/cslav.htm";

    //    str = "/home/files/Develop/git/QDict4BQ6/QDict4BQ6/bin/Example/"
    str = "/home/files/Develop/git/QDict4BQ6/QDict4BQ6/bin/Example/dict_En_CJB.htm";
    ui->LEFile->setText(str);

    //    QString text = getTextFromHtmlFile(str);
    //    qDebug()  << "Author = " << getParamInfo(&text, "Author");
    ui->LENameDict->setText("text name1");
    ui->LEAuthor->setText("test");

    setInfoDictFromFile();
    //    showHideEdit(1);

    //    QStringList list = getListWord(ui->LEFile->text());
    //    qDebug() << list;
    //    getParams();
}
///----------------------------------------------------------------------------
void MainWidget::init()
{
    /// set data
    setLangList();
    setTypeList();

    gui_about = new AboutDialog(this);

    gui_settings = new AppSettings(this);
    gui_settings->loadSettings();

    gui_htmleditor = new HtmlEditor(this);

    ui->LEArticles->setText("</h4>");
    ui->LEHeadLines->setText("<h4>");
    /// moved to center desktop
    QRect rect = QApplication::desktop()->availableGeometry(this);
    this->move(rect.width() / 2 - this->width() / 2,
               rect.height() / 2 - this->height() / 2);
    showHideEdit(ui->checkBEditOn->checkState());
    ui->tableEdit->setColumnCount(2);
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
    /// double click in table edit
    connect(ui->tableEdit, SIGNAL(cellDoubleClicked(int,int)), SLOT(editWordInPos(int,int)));
    /// saveWordInfo
    connect(gui_htmleditor, SIGNAL(saveWordInfo(QString)), SLOT(replaceStr(QString)));
    /// add word to table
    connect(ui->pBAddWord, SIGNAL(clicked()), SLOT(addWordToTable()));
    /// remove word from table
    connect(ui->pBRemove, SIGNAL(clicked()), SLOT(removeWordFromTable()));
    /// dynamic find
    connect(ui->LEFile, SIGNAL(textChanged(QString)), SLOT(find(QString)));
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
        setInfoDictFromFile();
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
//    find("");
}
///----------------------------------------------------------------------------
void MainWidget::find(QString text)
{
//    ui->tableEdit->findItems(text);
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
    }
    while (line.indexOf("<h4>") == -1
           and !streamInput.atEnd());

    QStringList list = getParams();
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

    str = "<html>\r\n";
    count += str.length();
    str = "<head>\r\n";
    count += str.length();
    str = QString("\t<meta name=\"Author\" content=\"%1\">\r\n").arg(list.at(0));
    count += str.length();
    str = QString("\t<meta name=\"Date\" content=\"DateOfDict\">\r\n");
    count += str.length();
    str = QString("\t<meta name=\"Date\" content=\"%1\">\r\n").arg(QDate::currentDate().toString());
    count += str.length();
    str = QString("\t<meta name=\"Revision\" content=\"%1\">\r\n").arg(list.at(1));
    count += str.length();
    str = QString("\t<meta name=\"Language\" content=\"%1\">\r\n").arg(list.at(2));
    count += str.length();
    str = QString("\t<meta name=\"Type\" content=\"%1\">\r\n").arg(list.at(3));
    count += str.length();
    str = QString("\t<meta name=\"Description\" content=\"%1\">\r\n").arg(list.at(4));
    count += str.length();
    str = QString("\t<meta name=\"Rights\" content=\"%1\">\r\n").arg(list.at(5));
    count += str.length();
    str = QString("\t<meta name=\"Numbering\" content=\"%1\">\r\n").arg(list.at(6));
    count += str.length();
    str = QString("<title>%1</title>\r\n").arg(ui->LENameDict->text());
    count += str.length();
    str = QString("</head>\r\n");
    count += str.length();


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
    /// create list word
    QStringList list = getListWord(ui->LEFile->text());

    QStringList listWord;
    QStringList listDescription;
    //   listWord = getParams();
    //   listDescription = getParams();

//        qDebug() << list;
    int ph4 = QString("<h4>").length();
    for (int i = 0; i < list.size(); i++)
    {
        QString line = list.at(i);

        int posBegin = line.indexOf("<h4>");
        int posEnd = line.indexOf("</h4>");
        listWord << line.mid(posBegin + ph4,
                             posEnd - ph4);
        listDescription << line.mid(posEnd + ph4 + 1,
                                    line.length() - posEnd - ph4-1);
    }

    /// clear table
//    ui->tableEdit->clearContents();
//    ui->tableEdit->repaint();
    for(int i = ui->tableEdit->rowCount(); i >= 0; i--)
    {
        ui->tableEdit->removeRow(i);
    }

    /// fill table
    for (int i = 0; i < listWord.size(); ++i)
    {
        QTableWidgetItem *wordNameItem = new QTableWidgetItem(listWord.at(i));
        wordNameItem->setFlags(wordNameItem->flags() ^ Qt::ItemIsEditable);

        QTableWidgetItem *descriptionItem = new QTableWidgetItem(listDescription.at(i));
        descriptionItem->setFlags(descriptionItem->flags() ^ Qt::ItemIsEditable);

        int row = ui->tableEdit->rowCount();
        ui->tableEdit->insertRow(row);

        ui->tableEdit->setItem(row, 0, wordNameItem);
        ui->tableEdit->setItem(row, 1, descriptionItem);
    }
    ui->tableEdit->resizeColumnsToContents();
    ui->tableEdit->resizeRowsToContents();
    ui->tableEdit->resizeColumnToContents(0);
    ui->tableEdit->resizeColumnToContents(1);
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
void MainWidget::setTypeList()
{
    QStringList items = getFillType();
    QStringListModel *typeModel = new QStringListModel(items, this);
    ui->comBoxType-> setModel(typeModel);
}
///----------------------------------------------------------------------------
void MainWidget::setLangList()
{
    QStringList items = getFillLang();
    QStringListModel *typeModel = new QStringListModel(items, this);
    ui->comBoxLang-> setModel(typeModel);
}
///----------------------------------------------------------------------------
void MainWidget::showHtmlEditor()
{
    gui_htmleditor->show();
}
///----------------------------------------------------------------------------
void MainWidget::editWordInPos(int row, int column)
{
    Q_UNUSED(row);
    //    qDebug() << "column = " << column
    //             << "row = " << row;
    if (column == 1)
    {
        QString old = ui->tableEdit->currentItem()->data(0).toString();
        //        qDebug() << old;

        QString path_to_file = QDir::currentPath()
                + "/edit.html";
        QFile::remove(path_to_file);

        //        qDebug() << path_to_file;
        createEmptyHtml(path_to_file, "edit", old);
        gui_htmleditor->load(path_to_file);
        gui_htmleditor->show();
    }
}
///----------------------------------------------------------------------------
void MainWidget::replaceStr(QString newstr)
{
//        qDebug() << newstr;
    QString old = ui->tableEdit->currentItem()->data(0).toString();
    replaceStrInFile(ui->LEFile->text(),
                     old,
                     newstr);
    QFile::remove(QDir::currentPath() + "/edit.html");
    showWordInTable();
}
///----------------------------------------------------------------------------
void MainWidget::setInfoDictFromFile()
{
    QString filePath = ui->LEFile->text();
    QStringList list = getInfoFromFile(filePath);

    ui->LEAuthor->setText(list.at(0));
    ui->spinBoxVersion->setValue(list.at(1).toDouble());
    ui->comBoxLang->setCurrentIndex(ui->comBoxLang->findText(list.at(2)));
    ui->comBoxType->setCurrentIndex(ui->comBoxType->findText(list.at(3)));
    ui->LEDescription->setText(list.at(4));
    ui->LECopyright->setText(list.at(5));
    ui->LENumbering->setText(list.at(6));
}
///----------------------------------------------------------------------------
void MainWidget::addWordToTable()
{
    QString wordName = ui->LEFind->text();
    QString wordDescription = "no description";

    QTableWidgetItem *wordNameItem = new QTableWidgetItem(wordName);
    wordNameItem->setFlags(wordNameItem->flags() ^ Qt::ItemIsEditable);

    QTableWidgetItem *descriptionItem = new QTableWidgetItem(wordDescription);
    descriptionItem->setFlags(descriptionItem->flags() ^ Qt::ItemIsEditable);

    int row = ui->tableEdit->rowCount();
    ui->tableEdit->insertRow(row);

    ui->tableEdit->setItem(row, 0, wordNameItem);
    ui->tableEdit->setItem(row, 1, descriptionItem);
    addWordToDict(ui->LEFile->text(), wordName, wordDescription);
}
///----------------------------------------------------------------------------
void MainWidget::removeWordFromTable()
{
    int column = ui->tableEdit->currentItem()->column();
    int row = ui->tableEdit->currentItem()->row();
//    qDebug() << "column = " << column
//             << "row = " << row;
    QString word, description;
    if (column == 0)
    {
        word = ui->tableEdit->item(row, column)->data(0).toString();
        description = ui->tableEdit->item(row, column +1)->data(0).toString();
    }
    if (column == 1)
    {
        description = ui->tableEdit->item(row, column)->data(0).toString();
        word = ui->tableEdit->item(row, column -1)->data(0).toString();
    }

    deleteWordFromDict(ui->LEFile->text(), word, description);
    showWordInTable();
//    qDebug() << "word = " << word
//             << "description = " << description;


}
///----------------------------------------------------------------------------
