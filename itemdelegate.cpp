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

#include "itemdelegate.h"


#include <QApplication>
#include <QModelIndex>
#include <QPainter>

#include <QDebug>
#include "documentview.h"
#include <QPaintDevice>
#include <QMutableHashIterator>
namespace {
    const int decorationMargin = 2;
}


///////////////////////////////////////////////////////////////////////////////
ItemDelegate::ItemDelegate(QObject *parent) : QAbstractItemDelegate(parent)
{
    mThumbnails=new QHash< QModelIndex,Thumbnail* > ;

}

ItemDelegate::~ItemDelegate()
{

    delete mThumbnails;
}

QString ItemDelegate::firstLine(const QModelIndex &index) const
{
    if (index.isValid())
        return index.model()->data(index, Qt::DisplayRole).toString();

    return QString();
}

QPixmap ItemDelegate::decoration(const QModelIndex &index) const
{
    if (!index.isValid()) return QPixmap();

    QImage image =imageThumbnail(index);

    return QPixmap::fromImage(image);

}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) return QSize();

    QFont normalfont = option.font;
    QFont boldfont = normalfont;
    boldfont.setBold(true);
    // Extract the items we want to measure
    QString firstRow = firstLine(index);

    // Compute the height
    QFontMetrics fm1(boldfont);
    QFontMetrics fm2(normalfont);
    int height = fm1.lineSpacing() + fm2.lineSpacing();
    height = qMax(height, option.decorationSize.height());
    // Compute the text width
    int width = fm1.width(firstRow);

    // Add decoration width + margin
    width += option.decorationSize.width()+decorationMargin;

    return QSize(width, height+20);
}


QPalette::ColorRole ItemDelegate::foregroundRole(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    if (option.state & QStyle::State_Selected) return QPalette::HighlightedText;
    return QPalette::Text;
}


void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if (!index.isValid())
        return;

    painter->save();

    QFont normalfont = option.font;
    QFont boldfont = normalfont;
    boldfont.setBold(true);
    QString firstRow = firstLine(index);
    QPixmap pixmap = decoration(index);
    QColor textcol = option.palette.color(foregroundRole(option, index));

    // Draw the background
    QStyleOptionViewItemV4 opt = option;
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
    int x,y;int rWidth=option.rect.width();
    // Draw the icon

    x = option.rect.left()+rWidth/2-(pixmap.width())/2;
    //        x = option.rect.left()+(option.decorationSize.width()-pixmap.width()+decorationMargin)/2;

    y = option.rect.top()+4;


    QRect pixmapRect = QStyle::visualRect(option.direction, option.rect, QRect(x, y, pixmap.width(), pixmap.height()));
    painter->drawPixmap(pixmapRect.x(), pixmapRect.y(), pixmap);
    painter->drawRect(pixmapRect);
    // Draw the text
    QFontMetrics fm1(boldfont);


    int y1;
    painter->setPen(textcol);

    if(fm1.width(firstRow)>=rWidth-decorationMargin){

        firstRow=fm1.elidedText(firstRow,Qt::ElideRight,rWidth-decorationMargin);

    }

    x = option.rect.left()+(rWidth)/2-(fm1.width(firstRow)/2);
    y1 = option.rect.top()+option.decorationSize.height()+4;

    QRect firstRowRect = QStyle::visualRect(option.direction, option.rect, QRect(x, y1, fm1.width(firstRow), fm1.lineSpacing()));
    // First line

    painter->drawText(firstRowRect, Qt::AlignCenter, firstRow);

    painter->restore();

}

QImage ItemDelegate::imageThumbnail(const QModelIndex index)const
{

    if(mThumbnails->contains(index)){

        Thumbnail *tb=mThumbnails->value(index);
        QImage image= tb->image();
        if(image.isNull())
            tb->startRender();

        return image;
    }

 int i=index.data(Qt::UserRole).toInt();

    Poppler::Page* page= mDocumentView->pageDoc(i);

    Thumbnail *tb=new Thumbnail(page,index);
    mThumbnails->insert(index,tb);
   // connect(tb,SIGNAL(imageFiniched(QModelIndex)),this,SLOT(updateImage(QModelIndex)));
    connect(tb,SIGNAL(imageFiniched(QModelIndex)),this,SIGNAL(sizeHintChanged(QModelIndex)));

    return tb->image();


}

//void ItemDelegate::updateImage(QModelIndex index)
//{
//    int i=index.data(Qt::UserRole).toInt();


//   // qDebug()<<"updateImage"<<i;
//   emit sizeHintChanged(index);
//}
