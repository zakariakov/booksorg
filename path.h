#ifndef PATH_H
#define PATH_H
#include<QObject>
#include<QSettings>
#include<QApplication>
#include<QDir>

class Path :public QObject
{
    Q_OBJECT

public:




    //! \brief setWorkingDir مجلد العمل
    //! \param path المسار
    //!
   static void setWorkingDir(const QString &dir){mWorkingDir=dir;}

    //! \brief workingDir مجلد العمل
    //! \return المسار
    //!
   static const QString workingDir()  { return mWorkingDir; }

    //! \brief thumbnailsDir مجلد المصغرات
    //! \return المسار
    //!
 static  const  QString thumbnailsDir(){return mWorkingDir+"/thumbnails/";}

   //! \brief booksDir مجلد الكتب
   //! \return المسار
   //!
  static  const QString booksDir()  {return mWorkingDir+"/books/";}

    //! \brief setCopyBooks نسخ الكتب
    //! \param copy نسخ نعم/لا
    //!
   static void setCopyBooks(bool copy){mCopy=copy;}

    //! \brief copyBooks نسخ الكتب
    //! \return  نعم/لا
    //!
   static bool copyBooks(){return mCopy;}


private:

   static QString mWorkingDir;
   static bool mCopy;
};

#endif // PATH_H
