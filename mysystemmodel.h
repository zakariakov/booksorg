#ifndef MYSYSTEMMODEL_H
#define MYSYSTEMMODEL_H

#include <QFileSystemModel>
#include "desktopappdata.h"
class MySystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit MySystemModel(QObject *parent = 0);
protected:
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int columnCount(const QModelIndex &) const{return 4;}

signals:
    
public slots:
    void setPath(const QString &fileName)const;
    QPixmap pixmap(const QModelIndex &index)const;
    QPixmap pixmapStars(const QModelIndex &index)const;

    QString title(const QModelIndex &index)const;
    QString title(const QString &path)const;
    QString comment(const QModelIndex &index)const;
    QString properties(const QModelIndex &index)const;
    QString author(const QModelIndex &index)const;
void updateAppDesktop(const QString &path);
private slots:


private :
    QHash<QString,DesktopAppData*>*hashData;
};

#endif // MYSYSTEMMODEL_H
