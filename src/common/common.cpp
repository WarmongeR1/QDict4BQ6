#include "common.h"


///----------------------------------------------------------------------------
QString getEncodingFromFile(QString file, QString language)
{
    QProcess pr;

#ifdef Q_OS_WIN
    pr.start("enca.exe -L " +language + " \""  + file + "\"");
#endif
#ifdef Q_OS_LINUX
    pr.start("enca -L " + language + " \""  + file + "\"");
#endif
    //    pr.start("pwd");

    QString encoding = "";
    if (pr.waitForFinished() == true)
    {
        QByteArray b = pr.readAll();
        encoding = QString(b);
    }
    if (encoding.indexOf("Universal transformation format 8 bits;") >= 0 ) encoding = "UTF-8";
    if (encoding.indexOf("Universal transformation format 16 bits;") >= 0 ) encoding = "UTF-16";
    if (encoding.indexOf("Universal transformation format 32 bits;") >= 0 ) encoding = "UTF-32";
    if (encoding.indexOf("Universal character set 2 bytes; UCS-2; BMP") >= 0) encoding = "UCS-2";
    if (encoding.indexOf("MS-Windows code page 1251") >= 0) encoding = "Windows-1251";
    if (encoding.indexOf("MS-Windows code page 1252") >= 0) encoding = "Windows-1252";
    if (encoding.indexOf("MS-Windows code page 1253") >= 0) encoding = "Windows-1253";
    if (encoding.indexOf("MS-Windows code page 1254") >= 0) encoding = "Windows-1254";
    if (encoding.indexOf("MS-Windows code page 1255") >= 0) encoding = "Windows-1255";
    if (encoding.indexOf("MS-Windows code page 1256") >= 0) encoding = "Windows-1256";
    if (encoding.indexOf("MS-Windows code page 1257") >= 0) encoding = "Windows-1257";
    if (encoding.indexOf("MS-Windows code page 1258") >= 0) encoding = "Windows-1258";
    if (encoding.indexOf("7bit ASCII characters") >= 0) encoding = "ASCII";
    if (encoding.indexOf("KOI8-R Cyrillic") >= 0) encoding = "KOI8-R";
    if (encoding.indexOf("KOI8-U Cyrillic") >= 0) encoding = "KOI8-U";
    if (encoding.indexOf("Unrecognized encoding") >= 0) encoding = "UTF-8";
    //    qDebug() << "encoding = " << encoding;
    return encoding;
}
///----------------------------------------------------------------------------
QString getFileNameAbs(const QString file)
{
    QStringList list;
    list << file.split("/");
    QString str = list.last();
    list = str.split(".");
    str = list.first();
    return str;
}
///----------------------------------------------------------------------------
QString getFileName(const QString file)
{
    QStringList list;
    list << file.split("/");
    QString str = list.last();
    return str;
}
///----------------------------------------------------------------------------
QString absolutifyFileName(QString fn, QString path)
{
    QString afn;
    if (!fn.isEmpty()){
        QDir dir(path);
        afn = dir.cleanPath( dir.absoluteFilePath(fn) );
    }
    return afn;
}
///----------------------------------------------------------------------------
QTextCodec * getCodecOfEncoding(QString encoding)
{
    //    encoding = encoding.toUpper ();
    QTextCodec * codec = QTextCodec::codecForName("UTF-8");
    if (encoding.toUpper ()== "UTF-8")        codec = QTextCodec::codecForName("UTF-8");
    if (encoding.toUpper ()== "UTF-16")       codec = QTextCodec::codecForName("UTF-16");
    if (encoding.toUpper ()== "UTF-16BE")       codec = QTextCodec::codecForName("UTF-16BE");
    if (encoding.toUpper ()== "UTF-16LE")       codec = QTextCodec::codecForName("UTF-16LE");
    if (encoding.toUpper ()== "UTF-32")       codec = QTextCodec::codecForName("UTF-32");
    if (encoding.toUpper ()== "UTF-32BE")       codec = QTextCodec::codecForName("UTF-32BE");
    if (encoding.toUpper ()== "UTF-32LE")       codec = QTextCodec::codecForName("UTF-32LE");
    if (encoding == "Windows-1251") codec = QTextCodec::codecForName("Windows-1251");
    if (encoding == "Windows-1252") codec = QTextCodec::codecForName("Windows-1252");
    if (encoding == "Windows-1253") codec = QTextCodec::codecForName("Windows-1253");
    if (encoding == "Windows-1254") codec = QTextCodec::codecForName("Windows-1254");
    if (encoding == "Windows-1255") codec = QTextCodec::codecForName("Windows-1255");
    if (encoding == "Windows-1256") codec = QTextCodec::codecForName("Windows-1256");
    if (encoding == "Windows-1257") codec = QTextCodec::codecForName("Windows-1257");
    if (encoding == "Windows-1258") codec = QTextCodec::codecForName("Windows-1258");
    if (encoding.toUpper ()== "KOI8-R")       codec = QTextCodec::codecForName("KOI8-R");
    if (encoding.toUpper ()== "KOI8-U")       codec = QTextCodec::codecForName("KOI8-U");

    //        qDebug() << " encoding = " << encoding;
    return codec;
}
///----------------------------------------------------------------------------
QStringList getFillLang()
{
    QStringList items;
    items << QString ("rus")
          << QString ("eng");
    return items;
}
///----------------------------------------------------------------------------
QStringList getFillType()
{
    //    Dictionary/Справочник
    //   Encyclopedia/Энциклопедия
    //   Glossary/Глоссарий
    //   Lexicon/Симфония
    //   Thesaurus/Тезаурус
    //   Vocabulary/Толковый
    //   Wordbook/Словарь
    QStringList items;
    items << QString ("Dictionary")
          << QString ("Encyclopedia")
          << QString ("Encyclopedia")
          << QString ("Glossary")
          << QString ("Lexicon")
          << QString ("Thesaurus")
          << QString ("Vocabulary")
          << QString ("Wordbook");
    return items;
}
///----------------------------------------------------------------------------
QStringList getListWord(QString filename)
{
    QStringList list;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream streamInput(&file);
        QString line = streamInput.readLine();
        int count;
        QString str, str2;
        do
        {
            /// edit to standart
            /// replace tag to standart
            //            line.replace(tagBegin, "<h4>");
            //            line.replace(tagEnd, "</h4>");

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
                //                streamDict << str;
            }

            if (!str2.isEmpty())
            {
                int count2;
                if (count %2 != 0)
                    count++;

                count2 = count;
                //                streamIdx << str2 << "\r\n" << QString::number(count2) + "\r\n";
            }
            count += (str.length()) *2;
            list << QString(str).remove("\r\n");
            line.remove(str);
            //            QString sttt = str2 + "<|>" + str;

            line.append(streamInput.readLine() + "\r\n");
        } while (!streamInput.atEnd());
        file.close();
    }
    else
    {
        qDebug() << "Error: not open file for read word list:" << filename;
    }
    return list;
}

///----------------------------------------------------------------------------
bool createEmptyHtml(QString fileName, QString title, QString text)
{
    bool ret = true;
    QFile file(fileName);
    if (!file.exists())
    {
        //create file if it's not exist
        if (file.open(QIODevice::ReadWrite))
        {
            //try to open or create file
            QTextStream ts(&file);
            ts.setCodec("UTF-8");
            ts << "<html>\n<head>" << endl;
            ts << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />" << endl;
            ts << "<title>" << title <<"</title>" << endl;
            ts << "</head>\n<body>\n" << text << "\n</body>\n</html>" << endl;
            file.close();
        }
        else
        {
            ret = false;
        }
    }
    return ret;
}
///----------------------------------------------------------------------------
bool createEmptyHtml(QString fileName, QString title)
{
    bool ret = true;
    QFile file(fileName);
    if (!file.exists())
    {
        //create file if it's not exist
        if (file.open(QIODevice::ReadWrite))
        {
            //try to open or create file
            QTextStream ts(&file);
            ts.setCodec("UTF-8");
            ts << "<html>\n<head>" << endl;
            ts << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />" << endl;
            ts << "<title>" << title <<"</title>" << endl;
            ts << "</head>\n<body>\n</body></html>" << endl;
            file.close();
        }
        else
        {
            ret = false;
        }
    }
    return ret;
}
///----------------------------------------------------------------------------
void replaceStrInFile(QString filepath, QString old, QString newstr)
{
    QFile file(filepath);
    QString str;
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        stream.setCodec(getCodecOfEncoding("UTF-8"));
        str = stream.readAll();
        str.replace(old, newstr);
    }
    else
    {
        qDebug() << "Error: not open file (replacestrinfile):" << filepath;
    }
    file.close();

    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream.setCodec(getCodecOfEncoding("UTF-8"));
        stream << str;
        file.close();
    }
    else
    {
        qDebug() << "Error: not open file (replacestrinfile write):" << filepath;
    }

}
///----------------------------------------------------------------------------
QString getTextFromHtmlFile(QString filePath)
{
    QString str = "";
    QFile file(filePath);
//    qDebug() << filePath;
    file.close();
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        stream.setCodec(getCodecOfEncoding("UTF-8"));
        str = stream.readAll();
//        qDebug() << "str = " << str;
        int body = QString("<body>").length();
        int posBegin = str.indexOf("<body>");

        int posEnd = str.indexOf("</body>");
        str = str.mid(posBegin + body,
                      posEnd - posBegin - body);

    }
    else
    {
        qDebug() << "Error: not open file(getTextFromHtmlFile):" << filePath;
    }

    return str;
}

///----------------------------------------------------------------------------
///----------------------------------------------------------------------------