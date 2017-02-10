/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#include <QtGui>

#include "pdfwidget.h"
Label::Label(QWidget *parent)
    : QLabel(parent)
{

}

PdfWidget::PdfWidget(QWidget *parent)
    : QScrollArea(parent),m_fitInwidth(0),isCostmColor(0)
{
    currentPage = -1;
    doc = 0;
   // rubberBand = 0;
    scaleFactor = 1.0;
    label=new Label;
  label-> setAlignment(Qt::AlignCenter);
 this->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenu()));

    this->setWidgetResizable(true);
    this->setContentsMargins(0, 0, 0, 0);
    this->setFrameShape(QFrame::NoFrame);
    this->setWidget(label);
  connect(label, SIGNAL(posChanged(int,int)), this, SLOT(scrollByMouse(int,int)));

}

PdfWidget::~PdfWidget()
{
 qDebug()<<"delete doc PDF;";
 if(doc)
    delete doc;
}
void PdfWidget::customContextMenu()
{


//        QAction *a;
        QMenu menu;

    menu.addActions(mListAction);
//        menu.addSeparator();
//        a=menu.addAction(QIcon::fromTheme("zoom-fit-page", QIcon(":/images/image/zoom-fit-width.png"))
//                          ,trUtf8("Fit Width"));
//        connect(a,SIGNAL(triggered(bool)),this,SLOT(fitInwidthPdf(bool)));
//        a->setCheckable(true);
//        a->setChecked(m_fitInwidth);
//        a=menu.addAction(QIcon::fromTheme("zoom-in"),tr("Zoom In"),this,SLOT(zoomIn()));
//        a=menu.addAction(QIcon::fromTheme("zoom-out"),tr("Zoom Out"),this,SLOT(zoomOut()));
//        a=menu.addAction(QIcon::fromTheme("zoom-original"),tr("Zoom Original"),this,SLOT(zoomOriginal()));

        menu.exec(QCursor::pos());

}

Poppler::Document *PdfWidget::document()
{
    return doc;
}
int PdfWidget::pagesCount()
{
    if(doc)
    return doc->numPages();

    return 0;
}
QMatrix PdfWidget::matrix() const
{
    return QMatrix(scaleFactor * physicalDpiX() / 72.0, 0,
                   0, scaleFactor * physicalDpiY() / 72.0,
                   0, 0);
}
   void PdfWidget::setEnsureVisible(QRectF rectF)
   {
       QRect rect=   matrix().mapRect(rectF).toRect();
       this->ensureVisible(rect.x(),rect.y());
   }

void Label::mousePressEvent(QMouseEvent *event)
{
//    if (!doc)
//        return;

    dragPosition = event->pos();
//    if (!rubberBand)
//        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
//    rubberBand->setGeometry(QRect(dragPosition, QSize()));
//    rubberBand->show();
}

void Label::mouseMoveEvent(QMouseEvent *event)
{
//    if (!doc)
//        return;

    int x=dragPosition.x()-event->x();
   int y=dragPosition.y()-event->y();
  // qDebug()<<x<<y;
    if(event->buttons()==Qt::LeftButton)
    emit posChanged(-x,y);
//    }else if(event->buttons()==Qt::MiddleButton){
//        if (y> 0)
//            emit requestZoomIn();
//        else
//            emit requestZoomOut();
//    }
  //  rubberBand->setGeometry(QRect(dragPosition, event->pos()).normalized());
}

void Label::mouseReleaseEvent(QMouseEvent *)
{
//    if (!doc)
//        return;

//    if (!rubberBand->size().isEmpty()) {
//        // Correct for the margin around the image in the label.
//        QRectF rect = QRectF(rubberBand->pos(), rubberBand->size());
//        rect.moveLeft(rect.left() - (width() - pixmap()->width()) / 2.0);
//        rect.moveTop(rect.top() - (height() - pixmap()->height()) / 2.0);
//        selectedText(rect);
//    }

   // rubberBand->hide();
}
/*
#ifndef QT_NO_WHEELEVENT
void PdfWidget::wheelEvent(QWheelEvent *event)
{
    // zoom in/out
    if (event->modifiers() == Qt::ControlModifier)
    {
        if (event->delta() > 0)
            emit requestZoomIn();
        else
            emit requestZoomOut();
    }

}
#endif // QT_NO_WHEELEVENT
*/
//qreal PdfWidget::scale() const
//{
//    return scaleFactor;
//}

void PdfWidget::showPage(int page)
{


    if (page != -1 && page != currentPage + 1) {
        currentPage = page - 1;
        emit pageChanged(page);
    }

    QImage image = doc->page(currentPage)
                      ->renderToImage(scaleFactor * physicalDpiX(), scaleFactor * physicalDpiY());

    if (!searchLocation.isEmpty()) {
        QRect highlightRect = matrix().mapRect(searchLocation).toRect();
        highlightRect.adjust(-2, -2, 2, 2);
      //  QImage highlight = image.copy(highlightRect);
        QPainter painter;
        painter.begin(&image);
//        painter.fillRect(image.rect(), QColor(0, 0, 0, 32));
//        painter.drawImage(highlightRect, highlight);
         painter.fillRect(highlightRect, QColor(0, 0, 0, 32));
        painter.end();
    }

   label-> setPixmap(QPixmap::fromImage(image));

}

QRectF PdfWidget::searchBackwards(const QString &text)
{
    QRectF oldLocation = searchLocation;

    int page = currentPage;
    if (oldLocation.isNull())
        page -= 1;

    while (page > -1) {

        QList<QRectF> locations;
        searchLocation = QRectF();

        while (doc->page(page)->search(text, searchLocation,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

            if (searchLocation != oldLocation)
                locations.append(searchLocation);
            else
                break;
        }

        int index = locations.indexOf(oldLocation);
        if (index == -1 && !locations.isEmpty()) {
            searchLocation = locations.last();
            showPage(page + 1);
            return searchLocation;
        } else if (index > 0) {
            searchLocation = locations[index - 1];
            showPage(page + 1);
            return searchLocation;
        }

        oldLocation = QRectF();
        page -= 1;
    }

    if (currentPage == doc->numPages() - 1)
        return QRectF();

    oldLocation = QRectF();
    page = doc->numPages() - 1;

    while (page > currentPage) {

        QList<QRectF> locations;
        searchLocation = QRectF();

        while (doc->page(page)->search(text, searchLocation,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

            locations.append(searchLocation);
        }

        if (!locations.isEmpty()) {
            searchLocation = locations.last();
            showPage(page + 1);
            return searchLocation;
        }
        page -= 1;
    }

    return QRectF();
}

QRectF PdfWidget::searchForwards(const QString &text)
{
    int page = currentPage;
    while (page < doc->numPages()) {

        if (doc->page(page)->search(text, searchLocation,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {
            if (!searchLocation.isNull()) {
                showPage(page + 1);
                setEnsureVisible(searchLocation);
                return searchLocation;
            }
        }
        page += 1;
        searchLocation = QRectF();
    }

    page = 0;

    while (page < currentPage) {

        searchLocation = QRectF();

        if (doc->page(page)->search(text, searchLocation,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {
            if (!searchLocation.isNull()) {
                showPage(page + 1);
                 setEnsureVisible(searchLocation);
                return searchLocation;
            }
        }
        page += 1;
    }

    return QRectF();
}

void PdfWidget::selectedText(const QRectF &rect)
{
    QRectF selectedRect = matrix().inverted().mapRect(rect);
    // QString text = doc->page(currentPage)->text(selectedRect);

    QString text;
    bool hadSpace = false;
    QPointF center;
    foreach (Poppler::TextBox *box, doc->page(currentPage)->textList()) {
        if (selectedRect.intersects(box->boundingBox())) {
            if (hadSpace)
                text += " ";
            if (!text.isEmpty() && box->boundingBox().top() > center.y())
                text += "\n";
            text += box->text();
            hadSpace = box->hasSpaceAfter();
            center = box->boundingBox().center();
        }
    }

    if (!text.isEmpty())
        emit textSelected(text);
}

bool PdfWidget::setDocument(const QString &filePath)
{

    // Poppler::Document *oldDocument = doc;
    doc = Poppler::Document::load(filePath);


    if (doc) {
        //  delete oldDocument;
        doc->setRenderHint(Poppler::Document::Antialiasing);
        doc->setRenderHint(Poppler::Document::TextAntialiasing);
        originColor=doc->paperColor().name();
        if(isCostmColor)
            doc->setPaperColor(QColor(costumColor));

        searchLocation = QRectF();
        currentPage = -1;
        scaleFactor = 1.0;
        scalPdf=1.0;
        label->setAlignment(Qt::AlignCenter);
        setPage(1);
        pdfWidth=label->pixmap()->width();
    }
    return doc != 0;
}

void PdfWidget::setPage(int page,QRectF rectF)
{
    if (page != currentPage + 1) {
        searchLocation = QRectF();
        showPage(page);
        setEnsureVisible(rectF);
    }
}

void PdfWidget::setScale(qreal scale)
{
    if (scaleFactor != scale) {
        scaleFactor = scale;
        showPage();
    }
}
void PdfWidget::loadToc(QTreeWidget *view)
{
     view->clear();
    if(!doc->toc()){

        return;
   }

    int count=doc->toc()->childNodes().count();
    if(count<1)return;

    for(int i=0;i<count;i++){
        QDomNode node=doc->toc()->childNodes().item(i);
        QString nN=  node.nodeName();
        QString iD=  node.attributes().item(0).firstChild().toText().data();
        QTreeWidgetItem *item=new QTreeWidgetItem(view) ;
        item->setText(0,nN);
         item->setText(1,iD.section(";",1,1));
        item->setData(0,1,iD);
        //qDebug()<<nN;
     // qDebug()<<iD<<"   "+nN;
        searchToc(node,item);

    }



}
void PdfWidget::searchToc(QDomNode node,QTreeWidgetItem *item)
{
    int count=node.childNodes().count();
    if(count<1)return;

    for(int r=0;r<count;r++){
        QDomNode nodeChild=node.childNodes().item(r);
        QString nN=  nodeChild.nodeName();
        QString iD=  nodeChild.attributes().item(0).firstChild().toText().data();
      //  qDebug()<<"----"+iD;
        QTreeWidgetItem *itemChild=new QTreeWidgetItem(item) ;
        itemChild->setText(0,nN);
        itemChild->setData(0,1,iD);
        itemChild->setText(1,iD.section(";",1,1));
        searchToc(nodeChild,itemChild);
    }

}
void PdfWidget::    scrollByMouse(int x,int y)
{
    int vx=this->horizontalScrollBar()->value();
    int vy=this->verticalScrollBar()->value();
    this->horizontalScrollBar()->setValue(vx+x);
    this->verticalScrollBar()->setValue(vy+y);
    qDebug()<<y<<vy<<this->verticalScrollBar()->maximum();
}
void PdfWidget::zoomOut()
{
qDebug()<<scalPdf;
        if(scalPdf<=0.2)return;
        scalPdf=scalPdf-0.10;
        if(doc)
        this->setScale(scalPdf);
        m_fitInwidth=false;
 emit fitWitdhChanged(false);

}

void PdfWidget::zoomIn()
{
qDebug()<<scalPdf;
        if(scalPdf>=4.0)return;
        scalPdf=scalPdf+0.10;
        if(doc)
        this->setScale(scalPdf);
        m_fitInwidth=false;
 emit fitWitdhChanged(false);

}
void PdfWidget::zoomOriginal()
{
qDebug()<<scalPdf;

        scalPdf=1.0;
        if(doc)
        this->setScale(scalPdf);
        m_fitInwidth=false;
        emit fitWitdhChanged(false);

}
void PdfWidget::fitInwidthPdf(bool fit)
{
    qDebug()<<"fit"<<scalPdf;
    m_fitInwidth=fit;
    if(m_fitInwidth==true){
    qreal w=this->width()-(this->verticalScrollBar()->width());


            scalPdf=w/   pdfWidth;;
            if(doc)
            this->setScale(scalPdf);
             emit fitWitdhChanged(true);
    }else{

//       if(doc)
//       this->setScale(scalPdf);
    }
}
