/*

Copyright 2012 Adam Reichold

This file is part of qpdfview.

qpdfview is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

qpdfview is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with qpdfview.  If not, see <http://www.gnu.org/licenses/>.

*/
/*
 This file is part of qpdfview.
 modified By AbouZakazia
*/

#include "pageitem.h"

QCache< PageItem*, QImage > PageItem::s_cache(32 * 1024 * 1024);

//bool PageItem::s_decoratePages = true;

QColor PageItem::s_backgroundColor(Qt::darkGray);
QColor PageItem::s_paperColor(Qt::white);

bool PageItem::s_invertColors = false;

//Qt::KeyboardModifiers PageItem::s_copyModifiers(Qt::ShiftModifier);
//Qt::KeyboardModifiers PageItem::s_annotateModifiers(Qt::ControlModifier);

int PageItem::cacheSize()
{
    return s_cache.totalCost();
}

void PageItem::setCacheSize(int cacheSize)
{
    s_cache.setMaxCost(cacheSize);
}

//bool PageItem::decoratePages()
//{
//    return s_decoratePages;
//}

//void PageItem::setDecoratePages(bool decoratePages)
//{
//    s_decoratePages = decoratePages;
//}



const QColor& PageItem::backgroundColor()
{
    return s_backgroundColor;
}

void PageItem::setBackgroundColor(const QColor& backgroundColor)
{
    if(backgroundColor.isValid())
    {
        s_backgroundColor = backgroundColor;
    }
}

const QColor& PageItem::paperColor()
{
    return s_paperColor;
}

void PageItem::setPaperColor(const QColor& paperColor)
{
    if(paperColor.isValid())
    {
        s_paperColor = paperColor;
    }
}

bool PageItem::invertColors()
{
    return s_invertColors;
}

void PageItem::setInvertColors(bool invertColors)
{
    s_invertColors = invertColors;

}

//const Qt::KeyboardModifiers& PageItem::copyModifiers()
//{
//    return s_copyModifiers;
//}

//void PageItem::setCopyModifiers(const Qt::KeyboardModifiers& copyModifiers)
//{
//    s_copyModifiers = copyModifiers;
//}

//const Qt::KeyboardModifiers& PageItem::annotateModifiers()
//{
//    return s_annotateModifiers;
//}

//void PageItem::setAnnotateModifiers(const Qt::KeyboardModifiers& annotateModifiers)
//{
//    s_annotateModifiers = annotateModifiers;
//}

PageItem::PageItem( Poppler::Page* page, int index, QGraphicsItem* parent) : QGraphicsObject(parent),
       m_page(0),
    m_index(-1),
    m_size(),
    m_links(),
    m_highlights(),
    m_rubberBandMode(ModifiersMode),
    m_rubberBand(),
    m_physicalDpiX(72),
    m_physicalDpiY(72),
    m_scaleFactor(1.0),
    m_rotation(Poppler::Page::Rotate0),
    m_transform(),
    m_normalizedTransform(),
    m_boundingRect(),
    m_image(),
    m_render(0)
{
    setAcceptHoverEvents(true);

    m_render = new QFutureWatcher< void >(this);
    connect(m_render, SIGNAL(finished()), SLOT(on_render_finished()));

    connect(this, SIGNAL(imageReady(int,int,qreal,Poppler::Page::Rotation,bool,QImage)), SLOT(on_imageReady(int,int,qreal,Poppler::Page::Rotation,bool,QImage)));


    m_page = page;

    m_index = index;
    m_size = m_page->pageSizeF();

    foreach(Poppler::Link* link, m_page->links())
    {
        if(link->linkType() == Poppler::Link::Goto)
        {
            if(!static_cast< Poppler::LinkGoto* >(link)->isExternal())
            {
                m_links.append(link);
                continue;
            }
        }
        else if(link->linkType() == Poppler::Link::Browse)
        {
            m_links.append(link);
            continue;
        }

        delete link;
    }




    prepareGeometry();
}

PageItem::~PageItem()
{
    m_render->cancel();
    m_render->waitForFinished();

    s_cache.remove(this);

    qDeleteAll(m_links);


    delete m_page;
}

QRectF PageItem::boundingRect() const
{
    return m_boundingRect;
}

void PageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QImage image;
  //  qDebug()<<"pauint";
    if(s_cache.contains(this))
    {
        image = *s_cache.object(this);
    }
    else
    {
        if(!m_image.isNull())
        {
            image = m_image;
            m_image = QImage();

            s_cache.insert(this, new QImage(image), image.byteCount());
        }
        else
        {
            startRender();
        }
    }

    // page

//    if(s_decoratePages)
//    {
        QColor paperColor = s_paperColor;

        if(s_invertColors)
        {
            paperColor.setRgb(~paperColor.rgb());
        }

        painter->fillRect(m_boundingRect, QBrush(paperColor));

        painter->drawImage(m_boundingRect.topLeft(), image);

        painter->setPen(QPen(s_invertColors ? Qt::white : Qt::black));
        painter->drawRect(m_boundingRect);
//    }
//    else
//    {
//        painter->drawImage(m_boundingRect.topLeft(), image);
//    }




    // highlights

    painter->save();

    painter->setTransform(m_transform, true);

    QColor highlightColor = QApplication::palette().color(QPalette::Highlight);

    highlightColor.setAlpha(127);

    foreach(QRectF highlight, m_highlights)
    {
        painter->fillRect(highlight.normalized(), QBrush(highlightColor));
    }

    painter->restore();

    // rubber band

    if(!m_rubberBand.isNull())
    {
        QPen pen;
        pen.setColor(s_invertColors ? Qt::white : Qt::black);
        pen.setStyle(Qt::DashLine);

        painter->setPen(pen);
        painter->drawRect(m_rubberBand);
    }
}
  QImage  PageItem::image()
  {

      QImage image;
    //  qDebug()<<"pauint";
      if(s_cache.contains(this))
      {
          image = *s_cache.object(this);
      }
      else
      {
          if(!m_image.isNull())
          {
              image = m_image;
              m_image = QImage();

              s_cache.insert(this, new QImage(image), image.byteCount());
          }
          else
          {
              startRender();
          }
      }
      return image;

  }
int PageItem::index() const
{
    return m_index;
}

const QSizeF& PageItem::size() const
{
    return m_size;
}

const QList< QRectF >& PageItem::highlights() const
{
    return m_highlights;
}

void PageItem::setHighlights(const QList< QRectF >& highlights, int duration)
{
    m_highlights = highlights;

    update();

    if(duration > 0)
    {
        QTimer::singleShot(duration, this, SLOT(clearHighlights()));
    }
}

PageItem::RubberBandMode PageItem::rubberBandMode() const
{
    return m_rubberBandMode;
}

void PageItem::setRubberBandMode(PageItem::RubberBandMode rubberBandMode)
{
    m_rubberBandMode = rubberBandMode;

    if(m_rubberBandMode == ModifiersMode)
    {
        unsetCursor();
    }
    else
    {
        setCursor(Qt::CrossCursor);
    }
}

int PageItem::physicalDpiX() const
{
    return m_physicalDpiX;
}

int PageItem::physicalDpiY() const
{
    return m_physicalDpiY;
}

void PageItem::setPhysicalDpi(int physicalDpiX, int physicalDpiY)
{
    if(m_physicalDpiX != physicalDpiX || m_physicalDpiY != physicalDpiY)
    {
        refresh();

        m_physicalDpiX = physicalDpiX;
        m_physicalDpiY = physicalDpiY;

        prepareGeometryChange();
        prepareGeometry();
    }
}

qreal PageItem::scaleFactor() const
{
    return m_scaleFactor;
}

void PageItem::setScaleFactor(qreal scaleFactor)
{
    if(m_scaleFactor != scaleFactor)
    {
        refresh();

        m_scaleFactor = scaleFactor;

        prepareGeometryChange();
        prepareGeometry();
    }
}

Poppler::Page::Rotation PageItem::rotation() const
{
    return m_rotation;
}

void PageItem::setRotation(Poppler::Page::Rotation rotation)
{
    if(m_rotation != rotation)
    {
        refresh();

        m_rotation = rotation;

        prepareGeometryChange();
        prepareGeometry();
    }
}

const QTransform& PageItem::transform() const
{
    return m_transform;
}

const QTransform& PageItem::normalizedTransform() const
{
    return m_normalizedTransform;
}

void PageItem::refresh()
{

    cancelRender();

    s_cache.remove(this);

    update();

}

void PageItem::clearHighlights()
{
    m_highlights.clear();

    update();
}

void PageItem::startRender(bool prefetch)
{
    if(!m_render->isRunning())
    {
        m_render->setFuture(QtConcurrent::run(this, &PageItem::render, m_physicalDpiX, m_physicalDpiY, m_scaleFactor, m_rotation, prefetch));
    }
}

void PageItem::cancelRender()
{
    m_render->cancel();

    m_image = QImage();
}

void PageItem::on_render_finished()
{
    update();
}

void PageItem::on_imageReady(int physicalDpiX, int physicalDpiY, qreal scaleFactor, Poppler::Page::Rotation rotation, bool prefetch, QImage image)
{
    if(m_physicalDpiX != physicalDpiX || m_physicalDpiY != physicalDpiY || !qFuzzyCompare(m_scaleFactor, scaleFactor) || m_rotation != rotation)
    {
        return;
    }

    if(s_invertColors)
    {
        image.invertPixels();
    }

    if(prefetch)
    {
        s_cache.insert(this, new QImage(image), image.byteCount());
    }
    else
    {
        if(!m_render->isCanceled())
        {
            m_image = image;
        }
    }
}

void PageItem::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
}

void PageItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    if(m_rubberBandMode == ModifiersMode && event->modifiers() == Qt::NoModifier)
    {
        // links

        foreach(Poppler::Link* link, m_links)
        {
            if(m_normalizedTransform.mapRect(link->linkArea().normalized()).contains(event->pos()))
            {
                if(link->linkType() == Poppler::Link::Goto)
                {
                    setCursor(Qt::PointingHandCursor);
                    QToolTip::showText(event->screenPos(), tr("Go to page %1.").arg(static_cast< Poppler::LinkGoto* >(link)->destination().pageNumber()));

                    return;
                }
                else if(link->linkType() == Poppler::Link::Browse)
                {
                    setCursor(Qt::PointingHandCursor);
                    QToolTip::showText(event->screenPos(), tr("Open %1.").arg(static_cast< Poppler::LinkBrowse* >(link)->url()));

                    return;
                }
            }
        }





       unsetCursor();
        QToolTip::hideText();
    }
}

void PageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
}

void PageItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // rubber band
/*
    if(m_rubberBandMode == ModifiersMode
            && (event->modifiers() == s_copyModifiers|| event->modifiers() == s_annotateModifiers) && event->button() == Qt::LeftButton)
    {
        setCursor(Qt::CrossCursor);

        if(event->modifiers() == s_copyModifiers)
        {
            m_rubberBandMode = CopyToClipboardMode;
        }

    }
*/
    if(m_rubberBandMode != ModifiersMode)
    {
        m_rubberBand = QRectF(event->pos(), QSizeF());

        emit rubberBandStarted();

        update();

        event->accept();
        return;
    }

    if(event->modifiers() == Qt::NoModifier && event->button() == Qt::LeftButton)
    {
        // links

        foreach(Poppler::Link* link, m_links)
        {
            if(m_normalizedTransform.mapRect(link->linkArea().normalized()).contains(event->pos()))
            {
               unsetCursor();

                if(link->linkType() == Poppler::Link::Goto)
                {
                    Poppler::LinkGoto* linkGoto = static_cast< Poppler::LinkGoto* >(link);

                    int page = linkGoto->destination().pageNumber();
                    qreal left = linkGoto->destination().isChangeLeft() ? linkGoto->destination().left() : 0.0;
                    qreal top = linkGoto->destination().isChangeTop() ? linkGoto->destination().top() : 0.0;

                    emit linkClicked(page, left, top);

                    event->accept();
                    return;
                }
                else if(link->linkType() == Poppler::Link::Browse)
                {
                    emit linkClicked(static_cast< Poppler::LinkBrowse* >(link)->url());

                    event->accept();
                    return;
                }
            }
        }





    }

    event->ignore();
}

void PageItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    emit sourceRequested(m_index + 1, m_transform.inverted().map(event->pos()));
}

void PageItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(!m_rubberBand.isNull())
    {
        if(m_boundingRect.contains(event->pos()))
        {
            m_rubberBand.setBottomRight(event->pos());

            update();
        }
    }
}

void PageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if(!m_rubberBand.isNull()&&m_rubberBand.width()!=-1&&m_rubberBand.height()!=-1)
    {
      //  unsetCursor();

        m_rubberBand = m_rubberBand.normalized();

        if(m_rubberBandMode == CopyToClipboardMode)
        {
            copyToClipboard(event->screenPos());
        }


      //  m_rubberBandMode = ModifiersMode;
        m_rubberBand = QRectF();
//setRubberBandMode(m_rubberBandMode);
        emit rubberBandFinished();

       update();

        event->accept();
        return;
    }

 update();
    event->ignore();
}


void PageItem::copyToClipboard(const QPoint& screenPos)
{
    QMenu* menu = new QMenu();

    QAction* copyTextAction = menu->addAction(tr("Copy &text"));
    QAction* copyImageAction = menu->addAction(tr("Copy &image"));
    QAction* saveImageToFileAction = menu->addAction(tr("Save image to &file..."));
    menu->addSeparator();
    QAction* addBookmarkAction = menu->addAction(tr("Add Bookmark"));
    QAction* action = menu->exec(screenPos);

    if(action == copyTextAction)
    {
        QString text;



        text = m_page->text(m_transform.inverted().mapRect(m_rubberBand));



        if(!text.isEmpty())
        {
            QApplication::clipboard()->setText(text);
        }
    }
    else if(action == copyImageAction || action == saveImageToFileAction)
    {
        QImage image;



        switch(m_rotation)
        {
        case Poppler::Page::Rotate0:
        case Poppler::Page::Rotate90:
            image = m_page->renderToImage(m_scaleFactor * m_physicalDpiX, m_scaleFactor * m_physicalDpiY, m_rubberBand.x(), m_rubberBand.y(), m_rubberBand.width(), m_rubberBand.height(), m_rotation);
            break;
        case Poppler::Page::Rotate180:
        case Poppler::Page::Rotate270:
            image = m_page->renderToImage(m_scaleFactor * m_physicalDpiY, m_scaleFactor * m_physicalDpiX, m_rubberBand.x(), m_rubberBand.y(), m_rubberBand.width(), m_rubberBand.height(), m_rotation);
            break;
        }


        if(!image.isNull())
        {
            if(action == copyImageAction)
            {
                QApplication::clipboard()->setImage(image);
            }
            else if(action == saveImageToFileAction)
            {
                QString fileName = QFileDialog::getSaveFileName(0, tr("Save image to file"), QDir::homePath(), "Portable network graphics (*.png)");

                if(!image.save(fileName, "PNG"))
                {
                    QMessageBox::warning(0, tr("Warning"), tr("Could not save image to file '%1'.").arg(fileName));
                }
            }
        }
    }else if(action==addBookmarkAction){

        QString  text = m_page->text(m_transform.inverted().mapRect(m_rubberBand));

        emit addBookmark(text);

    }

    delete menu;
}



void PageItem::prepareGeometry()
{
    m_transform.reset();
    m_normalizedTransform.reset();

    switch(m_rotation)
    {
    case Poppler::Page::Rotate0:
        break;
    case Poppler::Page::Rotate90:
        m_transform.rotate(90.0);
        m_normalizedTransform.rotate(90.0);
        break;
    case Poppler::Page::Rotate180:
        m_transform.rotate(180.0);
        m_normalizedTransform.rotate(180.0);
        break;
    case Poppler::Page::Rotate270:
        m_transform.rotate(270.0);
        m_normalizedTransform.rotate(270.0);
        break;
    }

    switch(m_rotation)
    {
    case Poppler::Page::Rotate0:
    case Poppler::Page::Rotate90:
        m_transform.scale(m_scaleFactor * m_physicalDpiX / 72.0, m_scaleFactor * m_physicalDpiY / 72.0);
        m_normalizedTransform.scale(m_scaleFactor * m_physicalDpiX / 72.0 * m_size.width(), m_scaleFactor * m_physicalDpiY / 72.0 * m_size.height());
        break;
    case Poppler::Page::Rotate180:
    case Poppler::Page::Rotate270:
        m_transform.scale(m_scaleFactor * m_physicalDpiY / 72.0, m_scaleFactor * m_physicalDpiX / 72.0);
        m_normalizedTransform.scale(m_scaleFactor * m_physicalDpiY / 72.0 * m_size.width(), m_scaleFactor * m_physicalDpiX / 72.0 * m_size.height());
        break;
    }

    m_boundingRect = m_transform.mapRect(QRectF(QPointF(), m_size));

    m_boundingRect.setWidth(qRound(m_boundingRect.width()));
    m_boundingRect.setHeight(qRound(m_boundingRect.height()));
}

void PageItem::render(int physicalDpiX, int physicalDpiY, qreal scaleFactor, Poppler::Page::Rotation rotation, bool prefetch)
{


    if(m_render->isCanceled() && !prefetch)
    {
        return;
    }

    QImage image;

    switch(rotation)
    {
    case Poppler::Page::Rotate0:
    case Poppler::Page::Rotate90:
        image = m_page->renderToImage(scaleFactor * physicalDpiX, scaleFactor * physicalDpiY, -1, -1, -1, -1, rotation);
        break;
    case Poppler::Page::Rotate180:
    case Poppler::Page::Rotate270:
        image = m_page->renderToImage(scaleFactor * physicalDpiY, scaleFactor * physicalDpiX, -1, -1, -1, -1, rotation);
        break;
    }

    if(m_render->isCanceled() && !prefetch)
    {
        return;
    }

    emit imageReady(physicalDpiX, physicalDpiY, scaleFactor, rotation, prefetch, image);
}
