#ifndef XCURSORTHEMEDATA_H
#define XCURSORTHEMEDATA_H
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QIcon>

#include <QString>
#include <QStringList>
#include <QSettings>
#include <QTextCodec>

#define thumbnails QDir::homePath()+"/."+QApplication::applicationName()+"/thumbnails"

class DesktopAppData
{
public:
    enum ItemDataRole {
      // Note: use   printf "0x%08X\n" $(($RANDOM*$RANDOM))
      // to define additional roles.
      GenericRole=0x24A3DAF8
    };

DesktopAppData(const QString &filePath)
{
    updatAppData(filePath);
}
void updatAppData(const QString &filePath)
{
    QString path=filePath;
    QFileInfo fi(path);
    if(fi.isDir())
        path=path+"/.directory";

    QSettings setting(path,QSettings::IniFormat);
    setting.setIniCodec(QTextCodec::codecForName("UTF-8"));

    setting.beginGroup("Service");
    setTitle(setting.value("Name",fi.fileName()).toString());//from read pdf
    setAuthor(setting.value("Author").toString());

    QString icn=setting.value("Icon","pdf").toString();
    if(QFile::exists(thumbnails+"/"+icn))
        setIcon(QIcon(thumbnails+"/"+icn));
    else{
        if(fi.isDir())
            setIcon(QIcon::fromTheme(icn,QIcon::fromTheme("folder")).pixmap(256));
        else
            setIcon(QIcon::fromTheme(icn,QIcon::fromTheme("application-pdf")).pixmap(256));

    }

    setComment(setting.value("Comment","").toString());//from read pdf
    setPath(setting.value("Exec",path).toString());
    setProperties(setting.value("Properties","").toString());
    setStars(QString::number(setting.value("Stars",0).toInt()));
    setIconStars(QIcon(QString(":/icons/%1.png").arg(mStars)));
    setting.endGroup();
}
    const QString &author() const { return mAuthor; }
    const QString &title() const { return mTitle; }
    const QString &comment() const { return mComment; }
    const QString &path() const { return mPath; }
    const QString &properties() const { return mProperties; }
    QIcon icon() const{ return mIcon; }
    QIcon iconStars() const{ return mIconStars; }
    const QString &stars() const { return mStars; }

  void setAuthor(const QString &aut){mAuthor=aut;}
  void setTitle(const QString &title){mTitle=title;}
  void setComment(const QString &description){mComment=description;}
  void setIcon(const QIcon &icon){mIcon=icon;}
  void setPath(const QString &path){mPath=path;}
  void setProperties(const QString &p){mProperties=p;}
  void setStars(const QString &s){mStars=s;}
  void setIconStars(const QIcon &icon){mIconStars=icon;}

protected:
    QString mAuthor;
    QString mTitle;
    QString mComment;
    QIcon mIcon;
    QString mPath;
    QString mProperties;
    QString mStars;
    QIcon mIconStars;
};

#endif // XCURSORTHEMEDATA_H
