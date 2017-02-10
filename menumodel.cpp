
#include <QtCore/QDebug>

#include "menumodel.h"

#include <QtCore/QDir>
#include <QMimeData>
#include <QUrl>

//#define DUMP_FOUND_THEMES

///////////////////////////////////////////////////////////////////////////////
MenuModel::MenuModel (QObject *parent) : QAbstractTableModel(parent)
{
    //insertThemes();
//setSupportedDragActions(Qt::MoveAction);
}

MenuModel::~MenuModel()
{
    qDeleteAll(mList);
    mList.clear();
}

QVariant MenuModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= mList.count()) return QVariant();
    const DesktopAppData *theme = mList.at(index.row());
    // Text label
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case NameColumn: return theme->title();

        case GenericColumn: return theme->description();

        default: return QVariant();
        }
    }
    // Description for the first name column
    if (role == DesktopAppData::GenericRole && index.column() == NameColumn) return theme->description();
    if (role == Qt::StatusTipRole && index.column() == NameColumn) return theme->path();
    if (role == Qt::ToolTipRole && index.column() == NameColumn) return theme->title();
    // Icon for the name column
    if (role == Qt::DecorationRole && index.column() == NameColumn) return theme->icon();

    //
    return QVariant();
}

void MenuModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column);
    Q_UNUSED(order);
    // Sorting of the model isn't implemented, as the KCM currently uses
    // a sorting proxy model.
}

Qt::DropActions MenuModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags MenuModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList MenuModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData *MenuModel::mimeData(const QModelIndexList &indexes) const
 {
     QMimeData *mimeData = new QMimeData();

QList<QUrl>listurls;

     foreach (const QModelIndex &index, indexes) {
         if (index.isValid()) {
             QString text =data(index,  Qt::StatusTipRole).toString();// "/home/yahia/.gtkrc-2.0";//data(index, XCursorThemeData::DisplayDetailRole).toString();
             qDebug()<<text;
             listurls<<text;
         }
     }

  //   mimeData->setData("application/vnd.text.list", encodedData);
      mimeData->setUrls(listurls);
     return mimeData;
 }

//--------------------------------------------------------------------------
 DesktopAppData *MenuModel::desktopApplication(const QModelIndex &index)
{
    if (!index.isValid()) return NULL;
    if (index.row() < 0 || index.row() >= mList.count()) return NULL;
    return mList.at(index.row());

}
 //--------------------------------------------------------------------------
  DesktopAppData *MenuModel::desktopApplicationUrl(const QString &url)
 {

      for (int i = 0; i < mList.count(); ++i)
      {
         DesktopAppData *desktopApp = mList.at(i);
       if (desktopApp->path()== url) return desktopApp;
      }
      return NULL;

 }

bool MenuModel::addFile( DesktopAppData *desktopApp )
{

    // Append the theme to the list
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mList.append(desktopApp);

    endInsertRows();
    return true;
}

void MenuModel::removeFile(const QModelIndex &index)
{
    if (!index.isValid()) return;
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    delete mList.takeAt(index.row());
    endRemoveRows();
}

void MenuModel::removeAll()
{
 int count=   mList.count();
    for (int i = 0; i <count; ++i)
    {

      beginRemoveRows(QModelIndex(),i, i);

       delete mList.takeAt(0);
       endRemoveRows();

    }


}

QModelIndex MenuModel::findIndex(const QString &name)
{

    for (int i = 0; i < mList.count(); ++i)
    {
      const DesktopAppData *desktopApp = mList.at(i);
     if (desktopApp->path()== name) return index(i,i);
    }
    return QModelIndex();
}

  bool MenuModel::isGroup(const QModelIndex &index)
  {
      DesktopAppData *ds=desktopApplication(index);
      if(ds){
          QString type=ds->type();
          if(type=="Group")
              return true;
      }
      return false;
  }




