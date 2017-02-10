#include "mysystemmodel.h"
#include<QSettings>
#include<QDebug>

MySystemModel::MySystemModel(QObject *parent) :
    QFileSystemModel(parent)
{
    hashData=new QHash<QString,DesktopAppData*>;
}

QVariant MySystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();


    if(!hashData->contains(filePath(index)))
        hashData->insert(filePath(index),new DesktopAppData(filePath(index)));

    DesktopAppData *dData=hashData->value(filePath(index));

    if(index.column() == 0){

        if (role == Qt::DisplayRole)return dData->title();
        if (role == Qt::DecorationRole)return dData->icon();

    }else{

        if(index.column() == 1){
            if (role == Qt::DisplayRole)return dData->author();
        }else if(index.column() == 2){

            if (role == Qt::DisplayRole)return dData->comment();
        }else if(index.column() == 3){
            if(!fileInfo(index).isDir()){
                if (role == Qt::DisplayRole)return dData->stars();
                if (role == Qt::DecorationRole)return dData->iconStars();
            }else{
                if (role == Qt::DisplayRole)return QString();
            }
        }
    }

    return QFileSystemModel::data(index,role);
}

QVariant MySystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal)
    if(role == Qt::DisplayRole)
        switch(section)
        {
        case 0: return tr("Name");
        case 1: return tr("Author");
        case 2: return tr("Comment");
        case 3: return tr("Path");

            default: return QVariant();
        }

    return QVariant();
}

void MySystemModel::setPath(const QString &fileName)const
{

    QDirIterator it(fileName, QStringList("*.desktop"),
                    QDir::Files | QDir::NoDotAndDotDot);
    while (it.hasNext())
    {
         it.next();
         qDebug()<<it.filePath();
         if(!hashData->contains(it.filePath()))
             hashData->insert(it.filePath(),new DesktopAppData(it.filePath()));
    }

}

QPixmap MySystemModel::pixmap(const QModelIndex &index)const
{

    if(!index.isValid())return QPixmap();

    if(hashData->contains(filePath(index))){
        DesktopAppData *dData=hashData->value(filePath(index));
        if(dData)
            return QPixmap(dData->icon().pixmap(256));
    }
    return QPixmap();
}
QPixmap MySystemModel::pixmapStars(const QModelIndex &index)const
{

    if(!index.isValid())return QPixmap();

    if(hashData->contains(filePath(index))){
        DesktopAppData *dData=hashData->value(filePath(index));
        if(dData)
            return QPixmap(dData->iconStars().pixmap(256));
    }
    return QPixmap();
}

QString MySystemModel::title(const QModelIndex &index)const
{
    if(!index.isValid())return QString();

    if(hashData->contains(filePath(index))){
        DesktopAppData *dData=hashData->value(filePath(index));
        if(dData)
            return (dData->title());
    }
    return QString();
}
QString MySystemModel::title(const QString &path)const
{

    if(hashData->contains(path)){
        DesktopAppData *dData=hashData->value(path);
        if(dData)
            return (dData->title());
    }
    return QString();
}

QString MySystemModel::comment(const QModelIndex &index)const
{
    if(!index.isValid())return QString();
    if(hashData->contains(filePath(index))){
        DesktopAppData *dData=hashData->value(filePath(index));
        if(dData)
             if(!dData->comment().isEmpty())
            return (tr("Comment : ")+dData->comment());
    }
    return QString();
}

QString MySystemModel::properties(const QModelIndex &index)const
{
    if(!index.isValid())return QString();
    if(hashData->contains(filePath(index))){
        DesktopAppData *dData=hashData->value(filePath(index));
        if(dData)
            return (dData->properties());
    }
    return QString();
}

QString MySystemModel::author(const QModelIndex &index)const
{
    if(!index.isValid())return QString();
    if(hashData->contains(filePath(index))){
        DesktopAppData *dData=hashData->value(filePath(index));
        if(dData)
            if(!dData->author().isEmpty())
            return (tr("Author : ")+dData->author());
    }
    return QString();
}

void MySystemModel::updateAppDesktop(const QString &path)
{
    DesktopAppData *dData=hashData->value(path);
    if(dData)
       dData->updatAppData(path);
}
