/* Copyright © Abouzakaria
 * (c)GPL2 (c)GPL3
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

#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H
#include "documentview.h"
#include "thumbnail.h"
#include <QAbstractItemDelegate>
#include <QPainter>
#include <QCache>
#include <QList>
class QPainter;


class ItemDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
    
public:
    ItemDelegate(QObject *parent = 0);
    ~ItemDelegate();

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setView(DocumentView *view) {mDocumentView=view;}


private slots:


    //void updateImage(QModelIndex index);
private:

    QString firstLine(const QModelIndex &index) const;

    QPixmap decoration(const QModelIndex &index) const;

     QImage imageThumbnail(const QModelIndex index)const;

    QPalette::ColorRole foregroundRole(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    DocumentView *mDocumentView;

    QHash< QModelIndex,Thumbnail* > *mThumbnails;


};

#endif
