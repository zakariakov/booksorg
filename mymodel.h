/* Copyright © Abouzakaria
 * (c)GPL2 (c)GPL3
 *
 * This file is part of booksorg.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License version 2 or at your option version 3 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef MYMODEL_H
#define MYMODEL_H


#include <QStandardItemModel>
#include <QApplication>
#include <QDir>
#include <QSqlQuery>
#include <QSqlDatabase>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent>
#else
#include <QtConcurrentRun>
#endif // QT_VERSION

//! الاقسام
enum CateGory{
    CAT_GROUP,     /*مجموعة*/
    CAT_AUTHOR,    /*المؤلفين*/
    CAT_FAVORIT,   /*التنقيط*/
    CAT_SEARCH     /*بحث*/
};

class MyModel : public QStandardItemModel
{
    Q_OBJECT
public:

    explicit MyModel(QObject *parent = 0);
    //!
    //! \brief data
    //! \param index
    //! \param role
    //! \return
    //!
    QVariant data(const QModelIndex &index, int role) const;

    //! \brief category القسم
    //! \return   CAT_GROUP,CAT_AUTHOR,CAT_FAVORIT,CAT_SEARCH
    //!
    int category(){return mCategoryName;}

    //! \brief openDataBase فتح قاعدة البيانات
    //!
    void openDataBase();

protected:

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int columnCount(const QModelIndex &) const{return 5;}

signals:
    void showMessage(QString);
    void pathChanged(QString);
    void updateItem(QModelIndex);
public slots:

    //! \brief addBook اضافة كتاب الى قاعدة البيانات
    //! \param title العنوان
    //! \param author المؤلف
    //! \param group المجموعة
    //! \param icon اسم الايقونة
    //! \param comment التعريف
    //! \param properties الخصائص
    //! \param exec المسار
    //!
    void addBook(const QString &title, const QString &author,
                 const QString &group, const QString &icon,
                 const QString &comment, const QString &properties,
                 const QString &exec);

    //! \brief updateBook تحديث الكتاب
    //! \param index المعرف
    //! \param title العنوان
    //! \param author المؤلف
    //! \param group المجموعة
    //! \param comment التعريف
    //! \param path المسار
    //! \param favo التنقيط
    //! \param tag الوسم
    //!
    void updateBook(const int index, const QString &title,
                    const QString &author,
                    const QString &group,
                    const QString &comment,
                    const QString &path, const int &favo, const QString &tag);

    //! \brief updateCategiry تحديث المجموعة في حالة تغير الاعدادات
    //! \param oldTitle الاسم القديم
    //! \param title الاسم الجديد
    //!
    void updateCategiry(const QString &oldTitle, const QString &title);

    //! \brief isRoot هل المحدد مجموعة او كتاب
    //! \param index المعرف
    //! \return نعم/لا
    //!
    bool isRoot(const QModelIndex &index)const;

    //! \brief isCategory هل  المسار قسم
    //! \return نعم/لا
    //!
    bool isCategory(){return mIsCategory;}

    //! \brief refreshPath تحديث المسار
    //!
    void refreshPath();

    //! \brief setCategory تفعيل قسم
    //! \param cat القسم
    //!
    void setCategory(int cat);

    //! \brief openCategory فتح القسم المحدد
    //! \param title الاسم المحدد
    //!
    void openCategory(const QString & title);

    //! \brief searchTitle بحث عن عنوان
    //! \param serachStr نص البحث
    //! \param machCase حساس ام لا
    //!
    void searchTitle(const QString & serachStr,bool machCase=false);

    //! \brief searchTags بحث عن وسم
    //! \param serachStr نص البحث
    //!
    void searchTags(const QString & serachStr);

    //! \brief deleteSelectedIndexes
    //! \param selectedIndexes
    //!
    void deleteSelectedIndexes( const QModelIndexList selectedIndexes);

    //! \brief changeParent
    //! \param index
    //! \param parentName
    //! \param authorFromIndex
    //!
    void changeParent(const QModelIndex index, const QString &parentName, bool authorFromIndex);

    //! \brief titleFromIndex
    //! \param index
    //! \return
    //!
    QString titleFromIndex(const QModelIndex &index)const;

    //! \brief titleFromFile
    //! \param file
    //! \return
    //!
    QString titleFromFile(const QString &file);

    //! \brief commentFromIndex
    //! \param index
    //! \return
    //!
    QString commentFromIndex(const QModelIndex &index)const;

    //! \brief propertiesFromIndex
    //! \param index
    //! \return
    //!
    QString propertiesFromIndex(const QModelIndex &index)const;

    //! \brief authorFromIndex
    //! \param index
    //! \return
    //!
    QString authorFromIndex(const QModelIndex &index)const;

    //! \brief favoriteFromIndex
    //! \param index
    //! \return
    //!
    QString favoriteFromIndex(const QModelIndex &index)const;

    //! \brief pixmapFromIndex
    //! \param index
    //! \return
    //!
    QString pixmapNameFromIndex(const QModelIndex &index)const;

    //! \brief idFromIndex
    //! \param index
    //! \return
    //!
    QString idFromIndex(const QModelIndex &index)const;

   int idFromFile(const QString &file);

    //! \brief fileNameFromIndex
    //! \param index
    //! \return
    //!
    QString fileNameFromIndex(const QModelIndex &index) const;

    //! \brief groupFromIndex
    //! \param index
    //! \return
    //!
    QString groupFromIndex(const QModelIndex &index)const;

    //! \brief tagsFromIndex
    //! \param index
    //! \return
    //!
    QString tagsFromIndex(const QModelIndex &index)const;

    //! \brief page جلب الصفحة المفتوحة سابقا من كتاب
    //! \param file مسار الملف
    //! \return الصفحة
    //!
    int page(const QString &file);

    //! \brief savePage حفظ الصفحة الحالية للكتاب
    //! \param file مسار الملف
    //! \param page الصفحة
    //!
    void savePage(const QString &file, int page);

    //! \brief saveRating حفظ التنقيط
    //! \param rat النقطة
    //! \param file مسار الملف
    //!
    void saveRating(int rat, const QString &file);

    //! \brief authorList قائمة باسماء المؤلفين
    //! \return القائمة
    QStringList authorList()const {return mListAuthors;}

    //! \brief groupList قائمة باسماء المجموعات
    //! \return القائمة
    //!
    QStringList groupList()const {return mListGroups;}

    //! \brief rootPath
    //! \return
    //!
    QString rootPath() const  {return mRootPath;}

    //! \brief pathToString
    //! \return
    //!
    QString pathToString();

    //! \brief indexFromFileName
    //! \param file
    //! \return
    //!
    QHash<QString,QString>  indexFromFileName(const QString &file);

    //! \brief updateDataBase
    //!
    void updateDataBase();

    void cleanBooksDir();
private:

    enum Columns{
        ID,
        TITLE,
        AUTHOR,
        GROUP,
        FAVO,
        ICON,
        COMMENT,
        PROPERTY,
        EXEC,
        ISROOT ,
        TAGS
    };

    QSqlDatabase  db;
    int mCategoryName;
    bool mIsCategory;
    QStringList mListFavo;
    QStringList mListAuthors;
    QStringList mListGroups;
    QString mRootPath;

    QString mBooksPath;
    bool mCopyBooks;
bool mStop;
//QHash <QString,QIcon>hashIcons;
QHash<QString,QByteArray> hashIcons;


private slots:
    //! \brief iconPix
    //! \param iconName
    //! \return
    //!
    QByteArray iconPix(QString iconName);
void readIcon(QStandardItem *item);
void addItems(QSqlQuery query);
void readIcons(QList<QStandardItem *>list);
    QString checkFilePath(const QString &file);
    void init();

};

#endif // MYMODEL_H
