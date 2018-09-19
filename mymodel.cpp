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
#include "mymodel.h"
#include "path.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QPainter>
#include <QMessageBox>
#include <QFontInfo>
#include <QProgressDialog>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QDirIterator>
#include <QBuffer>
#include <QImageWriter>
//_____________________________________________________
MyModel::MyModel(QObject *parent) :
    QStandardItemModel(parent)
{

    insertColumns(0, 4);
    db = QSqlDatabase::addDatabase("QSQLITE");


sort(0);
}

void MyModel::openDataBase()
{
    db.setDatabaseName(Path::workingDir()+"/books.db");

    QString txt="no database: ";
    if(!db.open())
    {
        txt+=db.lastError().text();
        qDebug()<<"MyModel::openDataBase()"<<txt;
    }

    //! [Set up table  book]

    QSqlQuery query(db);

    //AUTOINCREMENT
    query.exec("CREATE TABLE 'main'.'books' ("
               "'id' INTEGER PRIMARY KEY  NOT NULL,"
               "'title' TEXT,"
               "'author' TEXT,"
               "'groups' TEXT,"
               "'favo' INTEGER NOT NULL DEFAULT (0),"
               "'icon' TEXT,"
               "'comment' TEXT,"
               "'properties' TEXT,"
               "'exec' TEXT,"
               "'page' INTEGER NOT NULL DEFAULT (1),"
                "'tags' TEXT"
                 ");");
  init();

}

//___________________________________________________________________
QVariant MyModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) return QVariant();
    /*
    if(!isRoot(index)){
        QStandardItem* curItem=item(index.row(),0);

        if(index.column() == 1){
             if (role == Qt::DisplayRole)return curItem->data(Qt::UserRole+AUTHOR);
        }else if(index.column() == 2){
            if (role == Qt::DisplayRole)return  curItem->data(Qt::UserRole +COMMENT);//comment
        }else if(index.column() == 3){

            if (role == Qt::DisplayRole)return  curItem->data(Qt::UserRole +FAVO).toString();//favo
            if (role == Qt::DecorationRole)
                return QIcon(QString(":/icons/%1.png")
                             .arg(curItem->data(Qt::UserRole +FAVO).toString()));

        }
    }
            QPixmap pic;
            pic.loadFromData(icon);

            item->setIcon(QIcon(pic));
            emit updateItem(this->indexFromItem(item));

    */
    if(!isRoot(index)){
    if(role == Qt::DecorationRole) {
        if(index.column() == 0) {
            QStandardItem* curitem=item(index.row(),0);
          QString icon=  curitem->data(Qt::UserRole+ICON).toString();
            if(hashIcons.contains(icon)){
             QPixmap pic;
            pic.loadFromData(hashIcons.value(icon));
            return QIcon(pic);
        }else{
                QPixmap pix(128,128);
              pix.fill(Qt::transparent);
                return QIcon(pix);

            }
        }
    }
    }
    return QStandardItemModel::data(index,role);

}

//____________________________________________________________________________________
QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if(orientation==Qt::Horizontal)
        if(role == Qt::DisplayRole)
            switch(section)
            {
            case 0: return tr("Name");
            case 1: return tr("Author");
            case 2: return tr("Comment");
            case 3: return tr("Group");
            case 4: return tr("Rating");
//            case 5: return tr("Path");
            default: return QVariant();
            }

    return QVariant();

}

//_____________________________________________________________________
void  MyModel::addBook(const QString &title,   const QString &author,
                       const QString &group,   const QString &icon,
                       const QString &comment, const QString &properties,
                       const QString &exec)
{

    int index=idFromFile(exec);
     qDebug()<<"MyModel::addBook() index :"<<index;
    if(index!=-1)
    {
        qDebug()<<"MyModel::addBook() This file exists :"<<index<<exec;
        QFile::remove(Path::thumbnailsDir()+icon);

        updateBook(index,title,author,group,comment,checkFilePath(exec),0,QString());
        return;
    }

    QString file=exec;

    if(Path::copyBooks()){

        QDir dir(Path::workingDir());
        dir.mkdir(Path::workingDir()+"/books");

        QFileInfo fi(exec);
        QString newFileName=Path::booksDir()+fi.fileName();

        if(QFile(newFileName).exists()){
            file=fi.fileName();
        }else{
            if(QFile::copy(exec,newFileName))
             file=fi.fileName();
        }


    }

    QSqlQuery query;

/*****************************************************************************
 *                                                                                                                                 *
 * insert columns into Books in order                                                                                 *
 * id=0 1=title 2=author  3=groups 4=favo  5=icon 6=comment 7=properties 8=exec  9=page*
 *                                                                                                                                  *
 * ****************************************************************************/

    QString txt=QString("insert into books values(NULL,'%1','%2','%3','%4','%5','%6','%7','%8','%9','%10')")
            .arg(title)
            .arg(author!=tr("Unknown") ? author:QString())
            .arg(group!=tr("Unknown") ? group:QString())
            .arg("0")
            .arg(icon)
            .arg(comment)
            .arg(properties)
            .arg(file)
            .arg("1")
            .arg(QString())
             ;

    query.exec(txt);

        if(!mListAuthors.contains(author)&& !author.isEmpty())
        mListAuthors.append(author);

    if(!mListGroups.contains(group) && !group.isEmpty())
        mListGroups.append(group);


}

//_____________________________________________________
void  MyModel::updateBook(const int index,const QString &title,
                          const QString &author,
                          const QString &group,
                          const QString &comment,
                          const QString &path,
                          const int &favo,
                         const QString &tag)
{


/* id=0 1=title 2=author  3=groups 4=favo  5=icon 6=comment 7=properties 8=exec  9=page   10=tags  */

    QString txt=QString("UPDATE books"
                        " SET title = '%1',"
                        "author = '%2',"
                        "groups = '%3',"
                        "comment = '%4',"
                        "exec = '%5',"
                        "favo = '%6',"
                        "tags = '%7'"
                        " WHERE id = '%8'")
            .arg(title)
            .arg(author!=tr("Unknown") ? author: QString())
            .arg(group!=tr("Unknown") ? group : QString())
            .arg(comment)
            .arg(path)
            .arg(favo)
            .arg(tag)
            .arg(index);

    if(!mListGroups.contains(group) && !group.isEmpty())
        mListGroups.append(group);

    if(!mListAuthors.contains(author) && !author.isEmpty())
        mListAuthors.append(author);

    QSqlQuery query(db);
    if(query.exec(txt))
        refreshPath();

}

//__________________________________________________________________________
void  MyModel::updateCategiry(const QString &oldTitle,const QString &title)
{
    QString column;
    switch (mCategoryName)
    {

    case CAT_GROUP: column="groups"; break;
    case CAT_FAVORIT:               return;
    case CAT_AUTHOR: column="author";break;
    default: break;

    }

    QString old_title=oldTitle;
    if(old_title==tr("Unknown"))
        old_title=QString();

    QString txt=QString("UPDATE books"
                        " SET %1 = '%2'"
                        " WHERE %1 = '%3'").arg(column)
            .arg(title).arg(old_title);

    QSqlQuery query(db);
    int i=-1;
    if(query.exec(txt)){
        switch (mCategoryName)
        {
        case CAT_GROUP:
            i=mListGroups.indexOf(old_title);
            if(i!=-1)
                mListGroups.replace(i,title);
            else
                mListGroups.append(title);
            break;

        case CAT_AUTHOR:
            i=mListAuthors.indexOf(old_title);
            if(i!=-1)
                mListAuthors.replace(i,title);
            else
                mListAuthors.append(title);
            break;
        }
        refreshPath();
    }
}

void MyModel::init()
{

    mListAuthors.clear();
    mListFavo.clear();
    mListGroups.clear();

    mListAuthors.append(tr("Unknown"));
    mListGroups.append(tr("Unknown"));

    for (int i = 0; i < 6; ++i)
        mListFavo.append(QString::number(i));

    QSqlQuery query;
    query.exec("SELECT DISTINCT author FROM books");

    while(query.next())
    {

        QString author=query.value(0).toString();
        if(!mListAuthors.contains(author)&& !author.isEmpty())
            mListAuthors.append(author);

    }

    query.exec("SELECT DISTINCT groups FROM books");

    while(query.next())
    {

        QString group=query.value(0).toString();

        if(!mListGroups.contains(group)&& !group.isEmpty())
            mListGroups.append(group);

    }



}

//_____________________________________________________
void MyModel::setCategory(int cat)
{
mStop=true;
    mIsCategory=true;
    mRootPath=QString();
    //  qDebug()<<invisibleRootItem()->rowCount();
    //    int count=invisibleRootItem()->rowCount();
    //    for (int i = 0; i < count; ++i) {
    //          invisibleRootItem()->removeRow(0);
    //    }
    clear();
    invisibleRootItem()->insertColumns(0, 6);
    QString icon;
    mCategoryName=cat;
    QStringList list;

    switch (cat) {
    case CAT_GROUP:
        list=mListGroups;
        icon="group128";
        break;

    case CAT_FAVORIT:
        list=mListFavo;
        icon="favo_";
        break;

    case CAT_AUTHOR:
        list=mListAuthors;
        icon="author128";
        break;

    default:
        break;
    }

    foreach (QString s, list)
    {
        if(s.isEmpty()||s.isNull())
            continue;
        QStandardItem* item = new QStandardItem();
        item->setText(s);

        if(mCategoryName==CAT_FAVORIT)
            item->setIcon(QIcon(QString(":/icons/%1.png").arg(icon+s)));
        else
            item->setIcon(QIcon(QString(":/icons/%1.png").arg(icon)));

        item->setData(true,Qt::UserRole+ISROOT);
        invisibleRootItem()-> appendRow(item);

    }

sort(0);
    emit pathChanged(pathToString());
}

//_____________________________________________________
void MyModel::openCategory(const QString & title)
{
    mStop=false;
    mIsCategory=false;
    mRootPath=title;
    QString column;
    switch (mCategoryName)
    {
    case CAT_GROUP:
        column="groups";
        break;
    case CAT_FAVORIT:
        column="favo";
        break;
    case CAT_AUTHOR:
        column="author";
        break;
    default:
        break;
    }

    clear();

    QString txt=QString("select * from books WHERE %1='%2'")
            .arg(column).arg(title!=tr("Unknown") ? title:QString());
    qDebug()<<"MyModel::setRootPath Query exec :"<<txt;
    QSqlQuery query(txt);
    addItems(query);
    emit pathChanged(pathToString());
}

//_____________________________________________________
void MyModel::searchTitle(const QString & serachStr,bool machCase)
{
    mIsCategory=false;
    mRootPath=QString();

    clear();

    QString txt;
    if(machCase){
        txt=QString("select * from books WHERE title='%1'")
                .arg(serachStr);
    }else{
        txt=QString("select * from books WHERE title like '%%1%'")
                .arg(serachStr);
    }

    QSqlQuery query(txt);
     addItems(query);
/*
   QList<QStandardItem*>list;
   while(query.next())
    {

        int id=query.value(ID).toInt();
        QString title=query.value(TITLE).toString();
        QString author=query.value(AUTHOR).toString();
        QString icon=query.value(ICON).toString();
        QString properties=query.value(PROPERTY).toString();
        QString exec=query.value(EXEC).toString();
        QString favo=query.value(FAVO).toString();
        QString group=query.value(GROUP).toString();
        QString comment=query.value(COMMENT).toString();
   QString tag=query.value(TAGS).toString();
        QStandardItem* item = new QStandardItem;
        item->setText(title);
       // item->setIcon(iconPix(icon));

        item->setData(false,Qt::UserRole+ISROOT);
        item->setData(exec,Qt::UserRole +EXEC);
        item->setData(icon,Qt::UserRole +ICON);
        item->setData(properties,Qt::UserRole +PROPERTY);
        item->setData(id,Qt::UserRole +ID);
        item->setData(tag,Qt::UserRole +TAGS);
        invisibleRootItem()->appendRow(QList<QStandardItem*>()<<item
                                       <<new QStandardItem(author)
                                       <<new QStandardItem(comment)
                                       <<new QStandardItem(group)
                                       <<new QStandardItem(QIcon(QString(":/icons/%1.png")
                                                                 .arg(favo)),favo)
                                       );
        //         invisibleRootItem()->appendRow(item);
        if(hashIcons.contains(icon)){
            QPixmap pic;
            pic.loadFromData(hashIcons.value(icon));

            item->setIcon(QIcon(pic));
        }else{
            QPixmap pix(128,128);
            pix.fill(Qt::transparent);
            item->setIcon(QIcon(pix));
            //   QtConcurrent::run(this,&MyModel::readIcon,item);
            list.append(item);
        }
    }
    if(list.count()>0)
        QtConcurrent::run(this,&MyModel::readIcons,list);
*/
    emit pathChanged(tr("Search/")+serachStr);



}

//_____________________________________________________
void MyModel::searchTags(const QString & serachStr)
{
    mIsCategory=false;
    mRootPath=QString();

    clear();

    QString    txt=QString("select * from books WHERE tags like '%%1%'")
            .arg(serachStr);

    QSqlQuery query(txt);

    addItems(query);
    emit pathChanged(tr("Search/")+serachStr);



}

void MyModel::addItems(QSqlQuery query)
{
     QList<QStandardItem*>list;
    while(query.next())
    {
    //    if(mStop)return;
        int id=query.value(ID).toInt();
        QString title=query.value(TITLE).toString();
        QString author=query.value(AUTHOR).toString();
        QString icon=query.value(ICON).toString();
        QString properties=query.value(PROPERTY).toString();
        QString exec=query.value(EXEC).toString();
        QString favo=query.value(FAVO).toString();
        QString group=query.value(GROUP).toString();
        QString comment=query.value(COMMENT).toString();
        QString tag=query.value(TAGS).toString();
        QStandardItem* item = new QStandardItem;
        item->setText(title);
        //    item->setIcon(iconPix(icon));

        item->setData(false,Qt::UserRole+ISROOT);
        item->setData(exec,Qt::UserRole +EXEC);
        item->setData(icon,Qt::UserRole +ICON);
        item->setData(properties,Qt::UserRole +PROPERTY);
        item->setData(id,Qt::UserRole +ID);
        item->setData(tag,Qt::UserRole +TAGS);

        invisibleRootItem()->appendRow(QList<QStandardItem*>()<<item
                                       <<new QStandardItem(author)
                                       <<new QStandardItem(comment)
                                       <<new QStandardItem(group)
                                       <<new QStandardItem(QIcon(QString(":/icons/%1.png")
                                                                 .arg(favo)),favo)
                                       /*<<new QStandardItem(exec)*/);
        //         invisibleRootItem()->appendRow(item);
        if(hashIcons.contains(icon)){
            QPixmap pic;
            pic.loadFromData(hashIcons.value(icon));

            item->setIcon(QIcon(pic));
        }else{
            QPixmap pix(128,128);
            pix.fill(Qt::transparent);
            item->setIcon(QIcon(pix));
            //   QtConcurrent::run(this,&MyModel::readIcon,item);
            list.append(item);
        }
        //
    }
    if(list.count()>0)
        QtConcurrent::run(this,&MyModel::readIcons,list);

}

void  MyModel::readIcon(QStandardItem *item)
{
//    QString iconName=item->data(Qt::UserRole +ICON).toString();
//    QIcon icon=iconPix(iconName);
//    hashIcons.insert(iconName,icon);
//    if(item){
//        item->setIcon(icon);
//        emit updateItem(this->indexFromItem(item));
//    }
}
void  MyModel::readIcons(QList<QStandardItem *>list)
{
    foreach (QStandardItem *item, list) {

        QString iconName=item->data(Qt::UserRole +ICON).toString();
        if(hashIcons.contains(iconName))
        continue;

        QByteArray icon=iconPix(iconName);
        hashIcons.insert(iconName,icon);
        if(item){
//            QPixmap pic;
//            pic.loadFromData(icon);

           // item->setIcon(QIcon(pic));
            emit updateItem(this->indexFromItem(item));
        }

    }

}
//_____________________________________________________
QByteArray MyModel::iconPix(QString iconName){
if(hashIcons.contains(iconName))
    return hashIcons.value(iconName);

    QImage pixIcon;
   pixIcon=  QImage(260,260,QImage::Format_ARGB32);
    QImage pix;
    pix.load(Path::thumbnailsDir()+iconName);
    pixIcon.fill(Qt::transparent);
   // p.begin(&pixIcon);
    QPainter p(&pixIcon);
    p.fillRect(0,0,259,259,QColor(229,239,255,120));

    QRect rect((260-pix.width())/2,(260-pix.height())/2,pix.width()-2,pix.height()-2);
    p.drawImage(rect,pix);

    p.setPen(Qt::lightGray);
    p.drawRect(0,0,260,260);
    p.setPen(Qt::gray);
    p.drawRect(-1,-1,259,259);
    p.setPen(Qt::gray);

    p.setOpacity(0.3);
    p.drawRect(-1,-1,260,260);

    //p.end();

    QBuffer buffer;
    QImageWriter writer(&buffer,"jpg");
    writer.setQuality(50);
    writer.write(pixIcon);
      return buffer.buffer();
   // return QIcon(QPixmap::fromImage(pixIcon));

}
//_________________________________________________________________________
void MyModel::changeParent( const QModelIndex index,const QString &parentName,bool author)
{

    QString column;
    if(author){
        column="author";
    }else{
        column="groups";
    }


    QStandardItem* curItem=item(index.row(),0);

    if(curItem)
    {

        QString id= curItem->data(Qt::UserRole+ID).toString();
        QString txt=QString("UPDATE books"
                            " SET '%1' = '%2'"
                            " WHERE id = '%3'")
                .arg(column)
                .arg(parentName!=tr("Unknown") ? parentName:QString())
                .arg(id);

        QSqlQuery query;
        query.exec(txt);

        qDebug()<<"MyModel::changeParent() query.exec:"<<txt;


        //update list
        if(author){
            if(!mListAuthors.contains(parentName))
                mListAuthors.append(parentName);
        }else{
            if(!mListGroups.contains(parentName))
                mListGroups.append(parentName);
        }

    }



}
//_________________________________________________________________________
//void MyModel::changeGroup( const QModelIndex index,const QString &group)
//{
//    QStandardItem* curItem=item(index.row(),0);

//    if(curItem)
//    {

//        QString id= curItem->data(Qt::UserRole+ID).toString();
//        QString txt=QString("UPDATE books"
//                            " SET groups = '%1'"
//                            " WHERE id = '%2'").arg(group)
//                .arg(id);
//        QSqlQuery query;
//        query.exec(txt);

//        qDebug()<<"Query changeGroup exec :"<<txt;
//    }
//}

//_________________________________________________________________________
void MyModel::deleteSelectedIndexes( const QModelIndexList selectedIndexes)
{
      bool removeBooks=false;

  /*  //----------------------------QMessageBox--qt>=v5.2---------------------------------

    QMessageBox msgBox;
    msgBox.setText( tr("Do you want to delete All selected itemes?."));
    //  msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::No);
    QCheckBox *checkbox = new QCheckBox(tr("Delete frome computer"));
          msgBox.setCheckBox(checkbox);

    int ret = msgBox.exec();
    removeBooks=checkbox->isChecked();
    delete checkbox;

    if(ret==QMessageBox::No)
        return;
    //----------------------------QMessageBox--------------------------------------
*/
  //------------------------dialogBox--------------------------
      QDialog *dialogBox=new QDialog;

      QVBoxLayout *verticalLayout = new QVBoxLayout(dialogBox);
      verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
      QLabel *label = new QLabel(dialogBox);
      verticalLayout->addWidget(label);

      QCheckBox *checkBox = new QCheckBox(dialogBox);
      verticalLayout->addWidget(checkBox);

      QDialogButtonBox *buttonBox = new QDialogButtonBox(dialogBox);
      buttonBox->setOrientation(Qt::Horizontal);
      buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Yes);

      verticalLayout->addWidget(buttonBox);

      QObject::connect(buttonBox, SIGNAL(accepted()), dialogBox, SLOT(accept()));
      QObject::connect(buttonBox, SIGNAL(rejected()), dialogBox, SLOT(reject()));

      dialogBox->setWindowTitle(QApplication::applicationName());
      label->setText(tr ("Do you want to delete All selected itemes?."));
      checkBox->setText(tr("Delete frome computer"));
       int ret = dialogBox->exec();
       removeBooks=checkBox->isChecked();
        delete dialogBox;

       if(ret==QDialog::Rejected)
           return;



   //------------------------dialogBox-----------------------------

    QList<int>list;
    foreach (QModelIndex index, selectedIndexes)
    {
        if(list.contains( index.row()))
            continue;

        list.append(index.row());

        QStandardItem* curItem=item(index.row(),0);

        if(!curItem)
            continue;
        //root items
        if(curItem->data(Qt::UserRole+ISROOT).toBool())
        {
            QString column;
            switch (mCategoryName)
            {
            case CAT_GROUP:  column="groups"; break;
            case CAT_FAVORIT: column="favo";  break;
            case CAT_AUTHOR:  column="author";break;
            default: break;

            }

            // التأكد من حذف كل الكتب في المجموعة
            QString title= curItem->text();
            if(title==tr("Unknown")) title=QString();

            QSqlQuery query;

            //----------------------------------------------------------1
            QString text1=QString("select * from books WHERE %1='%2'")
                    .arg(column).arg(title);
            query.exec(text1);
            while(query.next())
            {
                QString iconName=query.value(ICON).toString();
                //remove  image
                QFile::remove(Path::thumbnailsDir()+iconName);
                //remove book frome computer
                if(removeBooks){

                    QString bookName=query.value(EXEC).toString();

                    if(QFile::exists(Path::booksDir()+"/"+bookName))
                        QFile::remove(Path::booksDir()+"/"+bookName);
                    else if(QFile::exists(bookName))
                        QFile::remove(bookName);

                }

                qApp->processEvents();
            }
            //----------------------------------------------------------2
            QString text2=QString("DELETE FROM books WHERE %1='%2'")
                    .arg(column).arg(title);

            query.exec(text2);

            switch (mCategoryName)
            {
            case CAT_GROUP:mListGroups.removeOne(title); break;
            case CAT_AUTHOR: mListAuthors.removeOne(title); break;
            default:  break;

            }

        }
        else  //book item
        {

            QString id= curItem->data(Qt::UserRole+ID).toString();
            QString iconName=curItem->data(Qt::UserRole+ICON).toString();

            //delete item data
            QString txt=QString("DELETE FROM books WHERE id='%1'").arg(id);
            QSqlQuery query;
            query.exec(txt);
            //remove  image
            QFile::remove(Path::thumbnailsDir()+iconName);
            //remove book frome computer
            if(removeBooks){
                QString bookName=curItem->data(Qt::UserRole+EXEC).toString();

                if(QFile::exists(Path::booksDir()+"/"+bookName))
                    QFile::remove(Path::booksDir()+"/"+bookName);
                else if(QFile::exists(bookName))
                    QFile::remove(bookName);
            }

        }

    }

    refreshPath();

}

//_____________________________________________________
void MyModel::refreshPath()
{
    if(mIsCategory)
        setCategory(mCategoryName);
    else
        openCategory(mRootPath);
}

//_____________________________________________________INFORMATION
bool MyModel::isRoot(const QModelIndex &index) const
{

    if(!index.isValid())return false;
    QStandardItem* curItem=item(index.row(),0);
    return curItem->data(Qt::UserRole+ISROOT).toBool();

}
//_____________________________________________________INFORMATION
QString MyModel::titleFromFile(const QString &file)
{

    QString fileName=checkFilePath(file);
    QString   txt=QString("select title from books WHERE exec='%1'")
            .arg(fileName);
    QSqlQuery query;
    query.exec(txt);
    while(query.next())
    {
        return query.value(0).toString();
    }
    return QString();

}

//_____________________________________________________INFORMATION
QString MyModel::titleFromIndex(const QModelIndex &index)const
{

    if(!index.isValid())return QString();

    QStandardItem* curItem=item(index.row(),0);
    if(curItem)
        return curItem->text();
    return QString();

}


//_____________________________________________________INFORMATION
QString MyModel::authorFromIndex(const QModelIndex &index)const
{

    if(!index.isValid())return QString();

    QStandardItem* curItem=item(index.row(),1);
    if(curItem)
        return curItem->text();
    return QString();

}
//_____________________________________________________INFORMATION
QString MyModel::commentFromIndex(const QModelIndex &index)const
{

    if(!index.isValid())return QString();

    QStandardItem* curItem=item(index.row(),2);
    if(curItem)
        return curItem->text();
    return QString();

}
//_____________________________________________________INFORMATION
QString MyModel::groupFromIndex(const QModelIndex &index)const
{

    if(index.isValid()){
        QStandardItem* curItem=item(index.row(),3);
        if(curItem){
            QString name=curItem->text();
            return name=name!=QString()? name :tr("Unknown");
        }
    }
    return QString();

}
//_____________________________________________________INFORMATION
QString MyModel::favoriteFromIndex(const QModelIndex &index)const
{

    if(!index.isValid())return QString();
    QStandardItem* curItem=item(index.row(),4);
    if(curItem)
        return curItem->text();

    return QString();

}


//_____________________________________________________INFORMATION
QString MyModel::propertiesFromIndex(const QModelIndex &index)const
{

    if(!index.isValid())return QString();

    QStandardItem* curItem=item(index.row(),0);
    if(curItem)
        return curItem->data(Qt::UserRole+PROPERTY).toString();
    return QString();

}
//_____________________________________________________INFORMATION
QString MyModel::fileNameFromIndex(const QModelIndex &index)const
{
    if(!index.isValid())return QString();

    QStandardItem* curItem=item(index.row(),0);
    if(curItem)
        return curItem->data(Qt::UserRole+EXEC).toString();
    return QString();

}
//_____________________________________________________INFORMATION
QString MyModel::pixmapNameFromIndex(const QModelIndex &index)const
{

    if(!index.isValid())return QString();
    QStandardItem* curItem=item(index.row(),0);
    if(curItem)
        return curItem->data(Qt::UserRole+ICON).toString();
    return QString();

}
//_____________________________________________________INFORMATION
QString MyModel::idFromIndex(const QModelIndex &index)const
{

    if(!index.isValid())return QString();

    QStandardItem* curItem=item(index.row(),0);
    if(curItem)
        return curItem->data(Qt::UserRole+ID).toString();
    return QString();

}
//_____________________________________________________INFORMATION
int MyModel::idFromFile(const QString &file)
{

    QString fileName=checkFilePath(file);

        QString   txt=QString("select id from books WHERE exec='%1'")
                .arg(fileName);
        QSqlQuery query;
        query.exec(txt);
        while(query.next())
        {
            return query.value(0).toInt();
        }

        return -1;

}

//_____________________________________________________INFORMATION
int MyModel::page(const QString &file)
{
QString fileName=checkFilePath(file);

    QString   txt=QString("select page from books WHERE exec='%1'")
            .arg(fileName);
    QSqlQuery query;
    query.exec(txt);
    while(query.next())
    {
        return query.value(0).toInt();
    }

    return 1;

}

//_____________________________________________________INFORMATION
QString MyModel::tagsFromIndex(const QModelIndex &index)const
{

    if(!index.isValid())return QString();

    QStandardItem* curItem=item(index.row(),0);
    if(curItem){
      //  qDebug()<<"tag------------"<< curItem->data(Qt::UserRole+TAGS).toString();
        return curItem->data(Qt::UserRole+TAGS).toString();

    }
    return QString();


}

//______________________________________________________________
void MyModel::savePage(const QString &file,int page)
{
    QString fileName=checkFilePath(file);

    QString txt=QString("UPDATE books"
                        " SET page = '%1'"
                        " WHERE exec = '%2'")
            .arg(page).arg(fileName);

   qDebug()<<"MyModel::savePage()) Query  exec :"<<txt;


    QSqlQuery query(db);
    if(query.exec(txt))
        qDebug()<<"MyModel::savePage() page saved:"<<page;
}

//__________________________________________________________
void MyModel::saveRating(int rat,const QString &file)
{
QString fileName=checkFilePath(file);

    QString txt=QString("UPDATE books"
                        " SET favo = '%1'"
                        " WHERE exec = '%2'")
            .arg(rat).arg(fileName);

   qDebug()<<"MyModel::saveRating() Query  exec :"<<file;


    QSqlQuery query(db);
    if(query.exec(txt)){
     refreshPath();
    }

}


//_____________________________________________________INFORMATION
QString MyModel::pathToString()
{
    QString path;
    switch (mCategoryName)
    {
    case CAT_GROUP:   path=tr("Groups"); break;
    case CAT_FAVORIT: path=tr("Ratings");break;
    case CAT_AUTHOR:  path=tr("Authors");break;
    case CAT_SEARCH:  path=tr("Search"); break;
    default: break;
    }

    return path+"/"+mRootPath;
}

//____________________________________________________________________
QHash<QString,QString> MyModel:: indexFromFileName(const QString &file)
  {

//    QString mFileName=file;

//    QString mFileNameInfo=QFileInfo(file).fileName();

//    if(QFile::exists(mBooksPath+"/books/"+mFileNameInfo))
//        mFileName=mFileNameInfo;


QString fileName=checkFilePath(file);

    QHash<QString,QString>hash;
      QString  txt=QString("select * from books WHERE exec='%1'")
                  .arg(fileName);
   
  
      QSqlQuery query(txt);
     qDebug()<<"yModel:: indexFromFileName() Query exec :"<<txt;
      while(query.next())
      {
          //    qDebug()<<"Query Search exec :"<<    hash["title"];

          hash["id"]=query.value(ID).toString();
          hash["title"]=query.value(TITLE).toString();
          hash ["author"]=query.value(AUTHOR).toString();
          hash ["icon"]=query.value(ICON).toString();
         // hash [properties]=query.value(PROPERTY).toString();
          hash ["exec"]=query.value(EXEC).toString();
          hash ["favo"]=query.value(FAVO).toString();
          hash ["group"]=query.value(GROUP).toString();
          hash ["comment"]=query.value(COMMENT).toString();
          hash ["tag"]=query.value(TAGS).toString();
          
          
      } 
      return hash;
  }


void MyModel::updateDataBase(/*const QString &newPath*/)
{
    QProgressDialog progress(tr("Create books information..."), tr("Cancel"), 0, -1, 0);
    progress.setWindowModality(Qt::WindowModal);
    progress.setCancelButton(0);
    progress.show();
    qApp->processEvents();
    //if current path != new path
//    if(Path::workingDir()!=newPath){

//        QDir dir(newPath);
//        dir.mkdir(newPath+"/books");
//        dir.mkdir(newPath+"/thumbnails");

//        Path::setWorkingDir(newPath);
//        openDataBase();
//        setCategory(mCategoryName);
//        qApp->processEvents();

//    }
    openDataBase();
    setCategory(mCategoryName);
    qApp->processEvents();

    if(Path::copyBooks()){
        //list for all filesName
        QStringList list;
        QString txt=QString("select exec from books");
        QSqlQuery query;
        query.exec(txt);
        while(query.next())
        {
            list<<query.value(0).toString();
        }

        progress.setMaximum(list.count());
        progress.setLabelText("Update files...");
        int i=0;
        foreach (QString file, list)
        {
            i++;
            progress.setValue(i);

            QFileInfo fi(file);

            QString newFile=Path::booksDir()+fi.fileName();

            if(!QFile::exists(newFile))
                QFile::copy(file,newFile);

            qApp->processEvents();

            if(file!=fi.fileName()){
                QString txt=QString("UPDATE books"
                                    " SET exec = '%1'"
                                    " WHERE exec = '%2'")
                        .arg(fi.fileName()).arg(file);
                query.exec(txt);
                qDebug()<<txt;
            }
        }

    }

}


QString MyModel::checkFilePath(const QString &file)
{
    QFileInfo fi(file);
    if(QFile::exists(Path::booksDir()+fi.fileName()))
        return fi.fileName();

    return file;
}

void MyModel::cleanBooksDir()
{

    QStringList listThumb;
    QStringList listbooks;
    QString booksDir=Path::booksDir();
    QString thumbDir=Path::thumbnailsDir();

    QString   txt="select icon, exec from books";
    QSqlQuery query;
    query.exec(txt);
    while(query.next())
    {
        listThumb.append( query.value(0).toString());
        listbooks.append( query.value(1).toString());
    }

    //Clean databae
    foreach (QString file, listbooks) {
        if(QFile::exists(file))
            continue;

        if(!QFile::exists(booksDir+"/"+file)){
            qDebug() << "no exist"<<booksDir+file;
            QString text=QString("DELETE FROM books WHERE exec='%1'")
                    .arg(file);

            emit showMessage(tr("removing... ")+file+tr(" from data.."));
            query.exec(text);
            qApp->processEvents();
        }

    }

    //Clean thumbnail
    QDirIterator it(thumbDir, QStringList() << "*.png");
    while (it.hasNext()) {
        QFileInfo f(it.next());
        if(!listThumb.contains(f.fileName())){

            qDebug() << f.fileName()<<f.absoluteFilePath();
            emit showMessage(tr("removing...")+f.fileName());
            QFile::remove(f.absoluteFilePath());
            qApp->processEvents();
        }
    }

    //Clean books
    QDirIterator itB(booksDir, QStringList() << "*.pdf");
    while (itB.hasNext()) {
        QFileInfo f(itB.next());
        if(!listbooks.contains(f.fileName())){
            qDebug() << f.fileName()<<f.absoluteFilePath();
            emit showMessage(tr("removing...")+f.fileName());
            QFile::remove(f.absoluteFilePath());
            qApp->processEvents();
        }
    }



    emit showMessage("");

}
