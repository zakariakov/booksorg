
#ifndef MENUMODEL_H
#define MENUMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QStringList>
#include "desktopappdata.h"
class QDir;
//class DsktopAppData;


// The two TableView/TreeView columns provided by the model
enum Columns { NameColumn = 0, GenericColumn};

class MenuModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MenuModel (QObject *parent = 0);
    ~MenuModel ();

    inline int columnCount (const QModelIndex &parent = QModelIndex()) const;
    inline int rowCount (const QModelIndex &parent = QModelIndex()) const;
       QVariant data (const QModelIndex &index, int role) const;
    void sort (int column, Qt::SortOrder order = Qt::AscendingOrder);

    /// Returns the desktopApplication at @p index.
    DesktopAppData *desktopApplication(const QModelIndex &index);
 DesktopAppData *desktopApplicationUrl(const QString &url);
    /// Returns the index for the desktopApplication with the internal name @p name,
    /// or an invalid index if no matching desktopApplication can be found.
    QModelIndex findIndex (const QString &name);

    bool isGroup(const QModelIndex &index);
    /// Adds the desktopApplication, and returns @a true if successful or @a false otherwise.
    bool addFile (DesktopAppData *desktopApp );
    void removeFile (const QModelIndex &index);
    void removeAll();

Qt::DropActions supportedDropActions() const;
Qt::ItemFlags flags(const QModelIndex &index) const;
QStringList mimeTypes() const;
QMimeData *mimeData(const QModelIndexList &indexes) const;

private:



private:
    QList<DesktopAppData *>mList;

};

int MenuModel::rowCount (const QModelIndex &) const
{
  return mList.count();
}


int MenuModel::columnCount (const QModelIndex &) const
{
  return 2;
}

#endif
