
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
#include "documentview.h"
#include "eicon.h"
#include "searchthread.h"
#include <QPaintDevice>


//#define thumbnails QDir::homePath()+"/."+QApplication::applicationName()+"/thumbnails"
#define SPACING 5.0
#define MINSCALE 0.1
#define MAXSCALE 10.0
#define ZOOMBY 0.1
#define HIGHL_DURATION 5000
#define PAGES_PERROW 3

Qt::KeyboardModifiers DocumentView::s_zoomModifiers(Qt::ControlModifier);
Qt::KeyboardModifiers DocumentView::s_rotateModifiers(Qt::ShiftModifier);
Qt::KeyboardModifiers DocumentView::s_horizontalModifiers(Qt::AltModifier);



DocumentView::DocumentView(QWidget* parent) : QGraphicsView(parent),
    mDocument(0),
    mFilePath(),
    mPagesNumber(-1),
    mCurrentPage(-1),
    mPrevPage(-1),
    m_returnToPage(),
    m_returnToLeft(),
    m_returnToTop(),
    mContinuousMode(true),
    mLayoutMode(TwoPagesMode),
    mScaleMode(FitToPageWidth),
    mScaleFactor(1.0),
    mRotation(Poppler::Page::Rotate0),
    mHighlightAll(false),
    mRubberBandMode(PageItem::ModifiersMode),
    mScene(0),
    mPages(),
    mHeightToIndex(),
    mHighlight(0),
//    mContentsModel(0),
    mResults(),
    mCurrentResult(mResults.end()),
    mSearchThread(0),
    mAnimation(0),
    m_isAnimated(false),
    isRubberPrivat(false),
    mFontsModel(0)
{
    mScene = new QGraphicsScene(this);



    setScene(mScene);

    setDragMode(QGraphicsView::ScrollHandDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setAcceptDrops(false);

    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(on_verticalScrollBar_valueChanged(int)));

    // highlight

    mHighlight = new QGraphicsRectItem();
    mPixmapItemAnim=new QGraphicsPixmapItem;
    mPixmapItemAnim->setVisible(false);
    mHighlight->setVisible(false);
    mScene->addItem(mPixmapItemAnim);
    mPixmapItemAnim->setZValue(0);
    mScene->addItem(mHighlight);

    QColor highlightColor = QApplication::palette().color(QPalette::Highlight);

    highlightColor.setAlpha(127);
    mHighlight->setBrush(QBrush(highlightColor));

    highlightColor.setAlpha(255);
    mHighlight->setPen(QPen(highlightColor));

    // search

    mSearchThread = new SearchThread(this);

    connect(mSearchThread, SIGNAL(resultsReady(int,QList<QRectF>)), SLOT(on_searchThread_resultsReady(int,QList<QRectF>)));

 //   connect(mSearchThread, SIGNAL(progressed(int)), SIGNAL(searchProgressed(int)));
    connect(mSearchThread, SIGNAL(finished()), SIGNAL(searchFinished()));
    connect(mSearchThread, SIGNAL(canceled()), SIGNAL(searchCanceled()));




}

DocumentView::~DocumentView()
{
    mSearchThread->cancel();
    mSearchThread->wait();

    qDeleteAll(mPages);

    delete mDocument;
}

//const QString& DocumentView::filePath() const
//{
//    return mFilePath;
//}

void DocumentView::setContinousMode(bool continousMode)
{


    if(mContinuousMode != continousMode)
    {
        mContinuousMode = continousMode;
        if(!mDocument)return;
        qreal left = 0.0, top = 0.0;
        saveLeftAndTop(left, top);

        prepareView(left, top);

        emit continousModeChanged(mContinuousMode);
    }
    mPixmapItemAnim->setVisible(!mContinuousMode);
}


void DocumentView::setLayoutMode(DocumentView::LayoutMode layoutMode)
{
    if(mLayoutMode != layoutMode && layoutMode >= 0 && layoutMode < NumberOfLayoutModes)
    {
        mLayoutMode = layoutMode;
        if(!mDocument)return;
        if(mCurrentPage != currentPageForPage(mCurrentPage))
        {
            mCurrentPage = currentPageForPage(mCurrentPage);

            emit currentPageChanged(mCurrentPage);
        }

        prepareScene();
        prepareView();

        emit layoutModeChanged(mLayoutMode);
    }
}


void DocumentView::setScaleMode(ScaleMode scaleMode)
{
    if(mScaleMode != scaleMode && scaleMode >= 0 && scaleMode < NumberOfScaleModes)
    {
        mScaleMode = scaleMode;
 if(!mDocument)return;
        qreal left = 0.0, top = 0.0;
        saveLeftAndTop(left, top);

        prepareScene();
        prepareView(left, top);

        emit scaleModeChanged(mScaleMode);
    }
}



void DocumentView::setScaleFactor(qreal scaleFactor)
{
    if(!qFuzzyCompare(mScaleFactor, scaleFactor) && scaleFactor >= MINSCALE && scaleFactor <= MAXSCALE)
    {
        mScaleFactor = scaleFactor;

        if(mScaleMode == ScaleFactor)
        {
            qreal left = 0.0, top = 0.0;
            saveLeftAndTop(left, top);

            prepareScene();
            prepareView(left, top);
        }

        emit scaleFactorChanged(mScaleFactor);
    }
}



void DocumentView::setRotation(Poppler::Page::Rotation rotation)
{
    if(mRotation != rotation)
    {
        mRotation = rotation;

        prepareScene();
        prepareView();

        emit rotationChanged(mRotation);
    }
}



void DocumentView::setHighlightAll(bool highlightAll)
{
    if(mHighlightAll != highlightAll)
    {
        mHighlightAll = highlightAll;

        if(mHighlightAll)
        {
            for(int index = 0; index < mPagesNumber; ++index)
            {
                mPages.at(index)->setHighlights(mResults.values(index));
            }
        }
        else
        {
            foreach(PageItem* page, mPages)
            {
                page->clearHighlights();
            }
        }

        emit highlightAllChanged(mHighlightAll);
    }
}

void DocumentView::setRubberBand(bool r)
{

    if(!r){

        isRubberPrivat=false;
        setRubberBandMode(PageItem::ModifiersMode);

    } else{

        isRubberPrivat=true;
        setRubberBandMode(PageItem::CopyToClipboardMode);

    }


}

void DocumentView::setRubberBandMode(PageItem::RubberBandMode rubberBandMode)
{
    if(mRubberBandMode != rubberBandMode)
    {
        mRubberBandMode = rubberBandMode;

        foreach(PageItem* page, mPages)
        {
            page->setRubberBandMode(mRubberBandMode);
        }

        //emit rubberBandModeChanged(m_rubberBandMode);
    }
}

bool DocumentView::searchWasCanceled() const
{
    return mSearchThread->wasCanceled();
}

int DocumentView::searchProgress() const
{
    return mSearchThread->progress();
}


void DocumentView::show()
{
    QGraphicsView::show();

    prepareView();
}

bool DocumentView::open(const QString& filePath)
{
    Poppler::Document* document = Poppler::Document::load(filePath);

    if(document != 0)
    {
        if(document->isLocked())
        {
            QString password = QInputDialog::getText(this, tr("Unlock %1").arg(QFileInfo(filePath).completeBaseName()), tr("Password:"), QLineEdit::Password);

            if(document->unlock(password.toLatin1(), password.toLatin1()))
            {
                delete document;
                return false;
            }
        }

        mFilePath = filePath;

        mPagesNumber = document->numPages();
        mCurrentPage = 1;

        m_returnToPage.clear();
        m_returnToLeft.clear();
        m_returnToTop.clear();

        prepareDocument(document);

        prepareScene();
        prepareView();

        emit filePathChanged(mFilePath);
        emit numberOfPagesChanged(mPagesNumber);
        emit currentPageChanged(mCurrentPage);
    }

    return document != 0;
}

bool DocumentView::refresh()
{
    Poppler::Document* document = Poppler::Document::load(mFilePath);

    if(document != 0)
    {
        if(document->isLocked())
        {
            QString password = QInputDialog::getText(this, tr("Unlock %1").arg(QFileInfo(mFilePath).completeBaseName()), tr("Password:"), QLineEdit::Password);

            if(document->unlock(password.toLatin1(), password.toLatin1()))
            {
                delete document;
                return false;
            }
        }

        qreal left = 0.0, top = 0.0;
        saveLeftAndTop(left, top);

        mPagesNumber = document->numPages();
        mCurrentPage = mCurrentPage <= mPagesNumber ? mCurrentPage : mPagesNumber;

        prepareDocument(document);

        prepareScene();
        prepareView(left, top);

        emit numberOfPagesChanged(mPagesNumber);
        emit currentPageChanged(mCurrentPage);
    }

    return document != 0;
}



void DocumentView::previousPage()
{
    int previousPage = mCurrentPage;

    switch(mLayoutMode)
    {
    default:
    case SinglePageMode:
        previousPage -= 1;
        break;
    case TwoPagesMode:
    case TwoPagesWithCoverPageMode:
        previousPage -= 2;
        break;
    case MultiplePagesMode:
        previousPage -= PAGES_PERROW;
        break;
    }

    previousPage = previousPage >= 1 ? previousPage : 1;

    jumpToPage(previousPage, false);
}

void DocumentView::nextPage()
{
    int nextPage = mCurrentPage;

    switch(mLayoutMode)
    {
    default:
    case SinglePageMode:
        nextPage += 1;
        break;
    case TwoPagesMode:
    case TwoPagesWithCoverPageMode:
        nextPage += 2;
        break;
    case MultiplePagesMode:
        nextPage += PAGES_PERROW;
        break;
    }

    nextPage = nextPage <= mPagesNumber ? nextPage : mPagesNumber;

    jumpToPage(nextPage, false);
}

void DocumentView::firstPage()
{
    jumpToPage(1);
}

void DocumentView::lastPage()
{
    jumpToPage(mPagesNumber);
}

void DocumentView::jumpToPage(int page, bool returnTo, qreal changeLeft, qreal changeTop, bool anim)
{
    if(!mDocument)return;

mPrevPage=mCurrentPage;


    if(page >= 1 && page <= mPagesNumber)
    {
        qreal left = 0.0, top = 0.0;
        saveLeftAndTop(left, top);

        if(mCurrentPage != currentPageForPage(page) || qAbs(left - changeLeft) > 0.01 || qAbs(top - changeTop) > 0.01)
        {
            if(returnTo)
            {
                m_returnToPage.push(mCurrentPage);
                m_returnToLeft.push(left); m_returnToTop.push(top);
            }

            mCurrentPage = currentPageForPage(page);

            prepareView(changeLeft, changeTop,anim);

            emit currentPageChanged(mCurrentPage, returnTo);
        }
    }
}

void DocumentView::jumpToHighlight(const QRectF& highlight)
{
    PageItem* page = mPages.at(mCurrentPage - 1);

    page->setHighlights(QList< QRectF >() << highlight, HIGHL_DURATION);

    disconnect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(on_verticalScrollBar_valueChanged(int)));
    centerOn(page->transform().mapRect(highlight).translated(page->pos()).center());
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(on_verticalScrollBar_valueChanged(int)));
}

void DocumentView::startSearch(const QString& text, bool matchCase)
{
    if(!mDocument)return;
    cancelSearch();

    QList< int > indices;

    for(int index = mCurrentPage - 1; index < mPagesNumber; ++index)
    {
        indices.append(index);
    }

    for(int index = 0; index < mCurrentPage - 1; ++index)
    {
        indices.append(index);
    }

    mSearchThread->start( mDocument, indices, text, matchCase);
}

void DocumentView::cancelSearch()
{
    mSearchThread->cancel();
    mSearchThread->wait();

    mResults.clear();
    mCurrentResult = mResults.end();

    foreach(PageItem* page, mPages)
    {
        page->clearHighlights();
    }

    prepareHighlight();
}

void DocumentView::findPrevious()
{
      if(!mDocument)return;
    if(mCurrentResult != mResults.end())
    {
        if(leftIndexForIndex(mCurrentResult.key()) == mCurrentPage - 1)
        {
            --mCurrentResult;
        }
        else
        {
            mCurrentResult = --mResults.upperBound(mCurrentPage - 1);
        }
    }
    else
    {
        mCurrentResult = --mResults.upperBound(mCurrentPage - 1);
    }

    if(mCurrentResult == mResults.end())
    {
        mCurrentResult = --mResults.end();
    }

    prepareHighlight();
}

void DocumentView::findNext()
{
    if(!mDocument)return;
    if(mCurrentResult != mResults.end())
    {
        if(leftIndexForIndex(mCurrentResult.key()) == mCurrentPage - 1)
        {
            ++mCurrentResult;
        }
        else
        {
            mCurrentResult = mResults.lowerBound(mCurrentPage - 1);
        }
    }
    else
    {
        mCurrentResult = mResults.lowerBound(mCurrentPage - 1);
    }

    if(mCurrentResult == mResults.end())
    {
        mCurrentResult = mResults.begin();
    }

    prepareHighlight();
}

void DocumentView::zoomIn()
{
    if(!mDocument)return;
    if(scaleMode() != ScaleFactor)
    {
        setScaleFactor(qMin(mPages.at(mCurrentPage - 1)->scaleFactor() + ZOOMBY, MAXSCALE));
        setScaleMode(ScaleFactor);
    }
    else
    {
        setScaleFactor(qMin(scaleFactor() + ZOOMBY, MAXSCALE));
    }
}

void DocumentView::zoomOut()
{
     if(!mDocument)return;
    if(scaleMode() != ScaleFactor)
    {
        setScaleFactor(qMax(mPages.at(mCurrentPage - 1)->scaleFactor() - ZOOMBY, MINSCALE));
        setScaleMode(ScaleFactor);
    }
    else
    {
        setScaleFactor(qMax(scaleFactor() - ZOOMBY, MINSCALE));
    }
}

void DocumentView::originalSize()
{

    setScaleFactor(1.0);
    setScaleMode(ScaleFactor);
}

void DocumentView::fitPageWith()
{

  setScaleMode(DocumentView::FitToPageWidth );
}
void DocumentView::fitPageSize()
{

  setScaleMode(DocumentView::FitToPageSize );
}

void DocumentView::rotateLeft()
{
    switch(rotation())
    {
    case Poppler::Page::Rotate0:
        setRotation(Poppler::Page::Rotate270);
        break;
    case Poppler::Page::Rotate90:
        setRotation(Poppler::Page::Rotate0);
        break;
    case Poppler::Page::Rotate180:
        setRotation(Poppler::Page::Rotate90);
        break;
    case Poppler::Page::Rotate270:
        setRotation(Poppler::Page::Rotate180);
        break;
    }
}

void DocumentView::rotateRight()
{
    switch(rotation())
    {
    case Poppler::Page::Rotate0:
        setRotation(Poppler::Page::Rotate90);
        break;
    case Poppler::Page::Rotate90:
        setRotation(Poppler::Page::Rotate180);
        break;
    case Poppler::Page::Rotate180:
        setRotation(Poppler::Page::Rotate270);
        break;
    case Poppler::Page::Rotate270:
        setRotation(Poppler::Page::Rotate0);
        break;
    }
}


void DocumentView::on_verticalScrollBar_valueChanged(int value)
{
    if(mContinuousMode)
    {
        QRectF visibleRect = mapToScene(viewport()->rect()).boundingRect();

        foreach(PageItem* page, mPages)
        {
            if(!page->boundingRect().translated(page->pos()).intersects(visibleRect))
            {
                page->cancelRender();
            }
        }

        QMap< qreal, int >::iterator lowerBound = mHeightToIndex.lowerBound(-value);

        if(lowerBound != mHeightToIndex.end())
        {
            int page = lowerBound.value() + 1;

            if(mCurrentPage != page)
            {
                mCurrentPage = page;
                if(!m_isAnimated)
                emit currentPageChanged(mCurrentPage);
            }
        }
    }
}

void DocumentView::on_searchThread_resultsReady(int index, QList< QRectF > results)
{
    if(mSearchThread->wasCanceled())
    {
        return;
    }

    while(!results.isEmpty())
    {
        mResults.insertMulti(index, results.takeLast());
    }

    if(mHighlightAll)
    {
        mPages.at(index)->setHighlights(mResults.values(index));
    }

    if(mResults.contains(index) && mCurrentResult == mResults.end())
    {
        findNext();
    }
}


void DocumentView::on_pages_linkClicked(int page, qreal left, qreal top)
{
    page = page >= 1 ? page : 1;
    page = page <= mPagesNumber ? page : mPagesNumber;

    left = left >= 0.0 ? left : 0.0;
    left = left <= 1.0 ? left : 1.0;

    top = top >= 0.0 ? top : 0.0;
    top = top <= 1.0 ? top : 1.0;

    jumpToPage(page, true, left, top);
}

void DocumentView::on_pages_linkClicked(const QString& url)
{
 int ret= QMessageBox::warning(0,"",tr("this action open external links.\n"
                             "%1.").arg(url),QMessageBox::Ok,QMessageBox::Cancel);

 if(ret==QMessageBox::Ok)
        QDesktopServices::openUrl(QUrl(url));

}

void DocumentView::on_pages_rubberBandFinished()
{

    if(!isRubberPrivat)
    setRubberBandMode(PageItem::ModifiersMode);

}


void DocumentView::resizeEvent(QResizeEvent* event)
{
   if(!mDocument)return;

    QGraphicsView::resizeEvent(event);

    if(mScaleMode != ScaleFactor)
    {
        qreal left = 0.0, top = 0.0;
        saveLeftAndTop(left, top);

        prepareScene();
        prepareView(left, top);
    }

}

void DocumentView::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if(!m_returnToPage.isEmpty())
        {
            jumpToPage(m_returnToPage.pop(), false, m_returnToLeft.pop(), m_returnToTop.pop());
        }

        event->accept();
        return;
    }

    if(!mContinuousMode && event->modifiers() == Qt::NoModifier)
    {
        if(event->key() == Qt::Key_PageUp && verticalScrollBar()->value() == verticalScrollBar()->minimum() && mCurrentPage != 1)
        {
            previousPage();

            verticalScrollBar()->setValue(verticalScrollBar()->maximum());

            event->accept();
            return;
        }
        else if(event->key() == Qt::Key_PageDown && verticalScrollBar()->value() == verticalScrollBar()->maximum() && mCurrentPage != currentPageForPage(mPagesNumber))
        {
            nextPage();

            verticalScrollBar()->setValue(verticalScrollBar()->minimum());

            event->accept();
            return;
        }
    }

    QGraphicsView::keyPressEvent(event);
}

void DocumentView::wheelEvent(QWheelEvent* event)
{
    if(event->modifiers() == s_zoomModifiers)
    {
        if(event->delta() > 0)
        {
            zoomIn();
        }
        else
        {
            zoomOut();
        }

        event->accept();
        return;
    }
    else if(event->modifiers() == s_rotateModifiers)
    {
        if(event->delta() > 0)
        {
            rotateLeft();
        }
        else
        {
            rotateRight();
        }

        event->accept();
        return;
    }
    else if(event->modifiers() == s_horizontalModifiers)
    {
        QWheelEvent wheelEvent(event->pos(), event->delta(), event->buttons(), Qt::AltModifier, Qt::Horizontal);
        QApplication::sendEvent(horizontalScrollBar(), &wheelEvent);

        event->accept();
        return;
    }
    else if(event->modifiers() == Qt::NoModifier)
    {
        if(!mContinuousMode)
        {

/*
            if(event->delta() > 0 && verticalScrollBar()->value() == verticalScrollBar()->minimum() && m_currentPage != 1)
            {
              //  previousPage();

                verticalScrollBar()->setValue(verticalScrollBar()->maximum());

                event->accept();

                return;
            }
            else if(event->delta() < 0 && verticalScrollBar()->value() == verticalScrollBar()->maximum() && m_currentPage != currentPageForPage(m_numberOfPages))
            {
              //  nextPage();

                verticalScrollBar()->setValue(verticalScrollBar()->minimum());

                event->accept();
                return;
            }
            */

        }
    }

    QGraphicsView::wheelEvent(event);
}

void DocumentView::contextMenuEvent(QContextMenuEvent* event)
{
    event->setAccepted(false);

    QGraphicsView::contextMenuEvent(event);

    if(!event->isAccepted())
    {
        event->setAccepted(true);

        QMenu* menu = new QMenu();

QAction *a;
        QAction* returnToPageAction = menu->addAction(tr("&Return to page %1").arg(!m_returnToPage.isEmpty() ? m_returnToPage.top() : -1));
        returnToPageAction->setShortcut(QKeySequence(Qt::Key_Return));
        returnToPageAction->setIcon(EIcon::icon(UNDO));
        returnToPageAction->setIconVisibleInMenu(true);
        returnToPageAction->setVisible(!m_returnToPage.isEmpty());
        menu->addSeparator();
        // next page---------
        a=menu->addAction(EIcon::icon(GO_NEXT)
                                        ,tr("&Next page"),this,SLOT(nextPage()));
        // previous page-----
         a=   menu->addAction(EIcon::icon(GO_PREVIOUS)
                                           ,tr("&Previous page"),this,SLOT(previousPage()));

        // first page---------
         a=   menu->addAction(EIcon::icon(GO_FIRST)
                                            ,tr("&First page"),this,SLOT(firstPage()));

        // last page---------
        a= menu->addAction(EIcon::icon(GO_LAST),tr("&Last page")
                                         ,this,SLOT(lastPage()));

         menu->addSeparator();
        a =  menu->addAction(EIcon::icon(RELOAD),tr("&Refresh"),this,SLOT(refresh()));


        QAction* action = menu->exec(event->globalPos());

        if(action == returnToPageAction)
        {
            jumpToPage(m_returnToPage.pop(), false, m_returnToLeft.pop(), m_returnToTop.pop());
        }

        delete menu;
    }
}

int DocumentView::currentPageForPage(int page) const
{
    int currentPage = -1;

    switch(mLayoutMode)
    {
    default:
    case SinglePageMode:
        currentPage = page;
        break;
    case TwoPagesMode:
        currentPage = page % 2 != 0 ? page : page - 1;
        break;
    case TwoPagesWithCoverPageMode:
        currentPage = page == 1 ? page : (page % 2 == 0 ? page : page - 1);
        break;
    case MultiplePagesMode:
        currentPage = page - ((page - 1) % PAGES_PERROW);
        break;
    }

    return currentPage;
}

int DocumentView::leftIndexForIndex(int index) const
{
    int leftIndex = -1;

    switch(mLayoutMode)
    {
    default:
    case SinglePageMode:
        leftIndex = index;
        break;
    case TwoPagesMode:
        leftIndex = index % 2 == 0 ? index : index - 1;
        break;
    case TwoPagesWithCoverPageMode:
        leftIndex = index == 0 ? index : (index % 2 != 0 ? index : index - 1);
        break;
    case MultiplePagesMode:
        leftIndex = index - (index % PAGES_PERROW);
        break;
    }

    return leftIndex;
}

int DocumentView::rightIndexForIndex(int index) const
{
    int rightIndex = -1;

    switch(mLayoutMode)
    {
    default:
    case SinglePageMode:
        rightIndex = index;
        break;
    case TwoPagesMode:
        rightIndex = index % 2 == 0 ? index + 1 : index;
        break;
    case TwoPagesWithCoverPageMode:
        rightIndex = index % 2 != 0 ? index + 1 : index;
        break;
    case MultiplePagesMode:
        rightIndex = index - (index % PAGES_PERROW) + PAGES_PERROW - 1;
        break;
    }

    rightIndex = rightIndex <= mPagesNumber - 1 ? rightIndex : mPagesNumber - 1;

    return rightIndex;
}

void DocumentView::saveLeftAndTop(qreal& left, qreal& top) const
{
    PageItem* page = mPages.at(mCurrentPage - 1);

    QRectF boundingRect = page->boundingRect().translated(page->pos());
    QPointF topLeft = mapToScene(viewport()->rect().topLeft());

    left = (topLeft.x() - boundingRect.x()) / boundingRect.width();
    top = (topLeft.y() - boundingRect.y()) / boundingRect.height();
}

void DocumentView::prepareDocument(Poppler::Document* document)
{


    cancelSearch();

    qDeleteAll(mPages);

    if(mDocument != 0)
    {
        delete mDocument;


    }

    mDocument = document;


    mDocument->setRenderHint(Poppler::Document::Antialiasing, true);
    mDocument->setRenderHint(Poppler::Document::TextAntialiasing, true);
    mDocument->setRenderHint(Poppler::Document::TextHinting, false);

    mDocument->setPaperColor(PageItem::paperColor());

#ifdef HAS_POPPLER_22

    m_document->setRenderHint(Poppler::Document::OverprintPreview, false);

#endif // HAS_POPPLER_22

    preparePages();
  //  prepareThumbnails();
   // prepareOutline();



}

void DocumentView::preparePages()
{
    mPages.clear();
    mPages.reserve(mPagesNumber);

    for(int index = 0; index < mPagesNumber; ++index)
    {

        PageItem* page = new PageItem( mDocument->page(index), index);
//qDebug()<<index;
        page->setPhysicalDpi(physicalDpiX(), physicalDpiY());
        page->setRubberBandMode(mRubberBandMode);

        mScene->addItem(page);
        mPages.append(page);

        connect(page, SIGNAL(linkClicked(int,qreal,qreal)), SLOT(on_pages_linkClicked(int,qreal,qreal)));
        connect(page, SIGNAL(linkClicked(QString)), SLOT(on_pages_linkClicked(QString)));

        connect(page, SIGNAL(rubberBandFinished()), SLOT(on_pages_rubberBandFinished()));
        connect(page, SIGNAL(addBookmark(QString)), SIGNAL(addBookmark(QString)));

      }

    QColor backgroundColor;

//    if(PageItem::decoratePages())
//    {
//        backgroundColor = PageItem::backgroundColor();
//    }
//    else
//    {
        backgroundColor = PageItem::paperColor();

        if(PageItem::invertColors())
        {
            backgroundColor.setRgb(~backgroundColor.rgb());
        }
//    }
//this->palette().background()
 //  mScene->setBackgroundBrush(QBrush(backgroundColor));
      mScene->setBackgroundBrush(QBrush(this->palette().dark()));
}

//PageItem* DocumentView::page(int index)
//{
//  return  mPages.at(index);
//}
 Poppler::Page* DocumentView::pageDoc(int index)
 {
     return mDocument->page(index);
 }

QImage DocumentView::imagePage(int index,QSizeF sizeF)
{



    PageItem* pageItem = mPages.at(index);
    return pageItem->image();

    Poppler::Page* page= mDocument->page(index);

    QSizeF pageSize = page->pageSizeF();//150

    qreal w=physicalDpiX() / 72.0 * pageSize.width();
    qreal h=physicalDpiX() / 72.0 * pageSize.height();

    qreal scaleFactor = qMin( sizeF.width()/w ,   sizeF.height()/h);

    QImage  image = page->renderToImage(scaleFactor * physicalDpiX(), scaleFactor * physicalDpiY());

    // qDebug()<<scaleFactor<<"image"<< image.size();
    return image;

}

//void DocumentView::prepareThumbnails()
//{
//    if(!mDocument)return;

//    mThumbnailsModel->clear();
//    mThumbnailsModel->invisibleRootItem()->insertColumns(0, 1);

//    for(int index = 0; index < mPagesNumber; ++index)
//    {
//        QApplication::processEvents();

//        QStandardItem* item = new QStandardItem();
//  QString num=this->locale().toString(index+1);
//        item->setText(num);
//        item->setData(index,Qt::UserRole);
//        //  item->setIcon(icon);
//        mThumbnailsModel->invisibleRootItem()->appendRow(item);

//    }
//}
/*
void DocumentView::prepareOutline()
{
   mContentsModel->clear();
mContentsModel->invisibleRootItem()->insertColumns(0, 0);

    QDomDocument* toc = mDocument->toc();

    if(toc != 0)
    {
        prepareOutline(toc->firstChild(), mContentsModel->invisibleRootItem());

        delete toc;
    }else{
        for(int index = 0; index < mPagesNumber; ++index)
        {
            QApplication::processEvents();

            QStandardItem* item = new QStandardItem();
            QString num=this->locale().toString(index+1);

            item->setText(tr("Page ")+num);
            item->setData(index+1, Qt::UserRole);
            mContentsModel->invisibleRootItem()->appendRow(item);

        }
    }

}
*/
void DocumentView::prepareOutline(const QDomNode& node, QStandardItem* parent)
{
    QDomElement element = node.toElement();

    QStandardItem* item = new QStandardItem();

  //  item->setFlags(Qt::ItemIsEnabled);

    item->setText(element.tagName());
    item->setToolTip(element.tagName());

    Poppler::LinkDestination* linkDestination = 0;

    if(element.hasAttribute("Destination"))
    {
        linkDestination = new Poppler::LinkDestination(element.attribute("Destination"));
    }
    else if(element.hasAttribute("DestinationName"))
    {
        linkDestination = mDocument->linkDestination(element.attribute("DestinationName"));
    }

    if(linkDestination != 0)
    {
        int page = linkDestination->pageNumber();
        qreal left = 0.0;
        qreal top = 0.0;

        page = page >= 1 ? page : 1;
        page = page <= mPagesNumber ? page : mPagesNumber;

        if(linkDestination->isChangeLeft())
        {
            left = linkDestination->left();

            left = left >= 0.0 ? left : 0.0;
            left = left <= 1.0 ? left : 1.0;
        }

        if(linkDestination->isChangeTop())
        {
            top = linkDestination->top();

            top = top >= 0.0 ? top : 0.0;
            top = top <= 1.0 ? top : 1.0;
        }

        item->setData(page, Qt::UserRole );
        item->setData(left, Qt::UserRole + 2);
        item->setData(top, Qt::UserRole + 3);

        delete linkDestination;
    }

  parent->appendRow(item);
//      parent->appendRow(QList<QStandardItem*>()<<item
  //                    <<new QStandardItem(item->data(Qt::UserRole + 1).toString()));
        QDomNode siblingNode = node.nextSibling();
    if(!siblingNode.isNull())
    {
        prepareOutline(siblingNode, parent);
    }

    QDomNode childNode = node.firstChild();
    if(!childNode.isNull())
    {
        prepareOutline(childNode, item);
    }
}



void DocumentView::prepareScene()
{
    // prepare scale factor and rotation

    for(int index = 0; index < mPagesNumber; ++index)
    {
        PageItem* page = mPages.at(index);
        QSizeF size = page->size();

        if(mScaleMode != ScaleFactor)
        {
            qreal visibleWidth = 0.0;
            qreal visibleHeight = 0.0;

            qreal pageWidth = 0.0;
            qreal pageHeight = 0.0;

            qreal scaleFactor = 1.0;

            switch(mLayoutMode)
            {
            default:
            case SinglePageMode:
                visibleWidth = viewport()->width() - 6.0 - 2.0 * SPACING;
                break;
            case TwoPagesMode:
            case TwoPagesWithCoverPageMode:
                visibleWidth = (viewport()->width() - 6.0 - 3 * SPACING) / 2;
                break;
            case MultiplePagesMode:
                visibleWidth = (viewport()->width() - 6.0 - (PAGES_PERROW + 1) * SPACING) / PAGES_PERROW;
                break;
            }

            visibleHeight = viewport()->height() - 2.0 * SPACING;

            switch(mRotation)
            {
            case Poppler::Page::Rotate0:
            case Poppler::Page::Rotate180:
                pageWidth = physicalDpiX() / 72.0 * size.width();
                pageHeight = physicalDpiY() / 72.0 * size.height();
                break;
            case Poppler::Page::Rotate90:
            case Poppler::Page::Rotate270:
                pageWidth = physicalDpiX() / 72.0 * size.height();
                pageHeight = physicalDpiY() / 72.0 * size.width();
                break;
            }

            switch(mScaleMode)
            {
            default:
            case ScaleFactor:
                break;
            case FitToPageWidth:
                scaleFactor = visibleWidth / pageWidth;
           //     qDebug()<<"visibleWidth"<<visibleWidth<<"pageWidth"<<pageWidth<<"size.width()"<<size.width();
                break;
            case FitToPageSize:
                scaleFactor = qMin(visibleWidth / pageWidth, visibleHeight / pageHeight);
                break;
            }

            page->setScaleFactor(scaleFactor);
        }
        else
        {
            page->setScaleFactor(mScaleFactor);
        }

      page->setRotation(mRotation);
    }

    // prepare layout

    mHeightToIndex.clear();

    qreal pageHeight = 0.0;

    qreal left = 0.0;
    qreal right = 0.0;
// qreal rightR = 0.0;

    qreal height = SPACING;

    for(int index = 0; index < mPagesNumber; ++index)
    {
        PageItem* page = mPages.at(index);
        QRectF boundingRect = page->boundingRect();

        switch(mLayoutMode)
        {
        default:
        case SinglePageMode:
            page->setPos(-boundingRect.left() - 0.5 * boundingRect.width(), height - boundingRect.top());

            mHeightToIndex.insert(-height + SPACING + 0.3 * pageHeight, index);

            pageHeight = boundingRect.height();

            left = qMin(left, -0.5f * boundingRect.width() - SPACING);
            right = qMax(right, 0.5f * boundingRect.width() + SPACING);
            height += pageHeight + SPACING;

            break;
        case TwoPagesMode:
        case TwoPagesWithCoverPageMode:
            if(index == leftIndexForIndex(index))
            {
                if(isRightToLeft())
                page->setPos(-boundingRect.left() + 0.5 * SPACING, height - boundingRect.top());
                 else
                page->setPos(-boundingRect.left() - boundingRect.width() - 0.5 * SPACING, height - boundingRect.top());

                mHeightToIndex.insert(-height + SPACING + 0.3 * pageHeight, index);

                pageHeight = boundingRect.height();

                left = qMin(left, -boundingRect.width() - 1.5f * SPACING);

                if(index == rightIndexForIndex(index))
                {
                    right = qMax(right, 0.5f * SPACING);
                    height += pageHeight + SPACING;
                }
            }
            else
            {
                if(isRightToLeft())
                    page->setPos(-boundingRect.left() - boundingRect.width() - 0.5 * SPACING, height - boundingRect.top());
                else
                    page->setPos(-boundingRect.left() + 0.5 * SPACING, height - boundingRect.top());


                pageHeight = qMax(pageHeight, boundingRect.height());

                right = qMax(right, boundingRect.width() + 1.5f * SPACING);
                height += pageHeight + SPACING;
            }

            break;
        case MultiplePagesMode:
            page->setPos(left /*- boundingRect.left()*/ + SPACING, height - boundingRect.top());
          //  page->setPos(right +  (boundingRect.width())  + SPACING, height - boundingRect.top());

            pageHeight = qMax(pageHeight, boundingRect.height());
           left += boundingRect.width() + SPACING;
          //  right -= boundingRect.width() + SPACING;

            if(index == leftIndexForIndex(index))
            {
                mHeightToIndex.insert(-height + SPACING + 0.3 * pageHeight, index);
            }

            if(index == rightIndexForIndex(index))
            {
                height += pageHeight + SPACING;
                pageHeight = 0.0;

                right = qMax(right, left + SPACING);
                left =0.0;
//                 right =  0.0;
//                 left=qMax(right, left + SPACING);
            }

            break;
        }
    }
//qDebug()<<right<<left;
    mScene->setSceneRect(left, 0.0, right - left, height);
//m_pagesScene->setSceneRect(left, 0.0, rightR  , height);
}

void DocumentView::prepareView(qreal changeLeft, qreal changeTop,bool anim)
{
    qreal left = mScene->sceneRect().left();
    qreal top = mScene->sceneRect().top();
    qreal width = mScene->sceneRect().width();
    qreal height = mScene->sceneRect().height();


    int horizontalValue = 0;
    int verticalValue = 0;

   // qDebug()<<"m_currentPage"<<mCurrentPage;

    for(int index = 0; index < mPages.count(); ++index)
    {
        PageItem* page = mPages.at(index);

        if(mContinuousMode)
        {
            page->setVisible(true);

            if(index == mCurrentPage - 1)
            {
                QRectF boundingRect = page->boundingRect().translated(page->pos());
                horizontalValue = qFloor(boundingRect.left() + changeLeft * boundingRect.width());
                verticalValue = qFloor(boundingRect.top() + changeTop * boundingRect.height());
            }
        }
        else
        {
            if(leftIndexForIndex(index) == mCurrentPage - 1)
            {

                page->setVisible(true);

                QRectF boundingRect = page->boundingRect().translated(page->pos());

                top = boundingRect.top() - SPACING;
                height = boundingRect.height() + 2.0 * SPACING;

                if(index == mCurrentPage - 1)
                {
                    horizontalValue = qFloor(boundingRect.left() + changeLeft * boundingRect.width());
                    verticalValue = qFloor(boundingRect.top() + changeTop * boundingRect.height());
                }

                //    if SinglePageMode &&
                //    continuousMode==false
                //    start animation
                if(mLayoutMode==SinglePageMode&&anim){

                    if(mPrevPage>-1 && mPrevPage-1<mPages.count()){
                        PageItem* curpage = mPages.at(mPrevPage-1);
                        if(curpage){
                            mPixmapItemAnim->setPixmap(QPixmap::fromImage(curpage->image()));
                           mPixmapItemAnim->setVisible(true);
                        }

                        if(mPrevPage<mCurrentPage)
                            animPage(page,true);
                        else if(mPrevPage>mCurrentPage)
                            animPage(page,false);
                    }

                }
                //end animation
            }
            else
            {
                page->setVisible(false);
                page->cancelRender();

            }
        }

        if(mCurrentResult != mResults.end())
        {
            if(mCurrentResult.key() == index)
            {
                mHighlight->setPos(page->pos());
                mHighlight->setTransform(page->transform());

                page->stackBefore(mHighlight);
            }
        }
    }

    setSceneRect(left, top, width, height);
    horizontalScrollBar()->setValue(horizontalValue);
    //verticalScrollBar()->setValue(verticalValue);

    if(mContinuousMode&&anim){
           // qDebug()<<"v:"<<verticalScrollBar()->value()<<verticalValue;
           animScrollbar(verticalScrollBar()->value(),verticalValue);
    //    verticalScrollBar()->setValue(verticalValue);
    }else
        verticalScrollBar()->setValue(verticalValue);

    viewport()->update();
}

void DocumentView::prepareHighlight()
{
    if(mCurrentResult != mResults.end())
    {
        jumpToPage(mCurrentResult.key() + 1);

        PageItem* page = mPages.at(mCurrentResult.key());

      mHighlight->setPos(page->pos());
        mHighlight->setTransform(page->transform());

       page->stackBefore(mHighlight);

        mHighlight->setRect(mCurrentResult.value().normalized());

        disconnect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(on_verticalScrollBar_valueChanged(int)));
        centerOn(mHighlight);
        connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(on_verticalScrollBar_valueChanged(int)));

        mHighlight->setVisible(true);
    }
    else
    {
        mHighlight->setVisible(false);
    }
}

void DocumentView::animPage( PageItem* page,bool top)
{


    QRectF boundingRect = page->boundingRect().translated(page->pos());
    mPixmapItemAnim->setPos(QPoint(boundingRect.x(),boundingRect.y()));

    QPoint start;
    if(top)
        start=QPoint(boundingRect.x(),boundingRect.top()+boundingRect.height());
    else
        start=QPoint(boundingRect.x(),boundingRect.top()-boundingRect.height());

    mAnimation = new QPropertyAnimation(page, "pos");
    connect(mAnimation,SIGNAL(finished()),this,SLOT(afterAnimation()));

    mAnimation->setDuration(300);
    mAnimation->setStartValue(start);

    mAnimation->setEndValue(QPoint(boundingRect.x(),boundingRect.y()));

    mAnimation->start();

}

void DocumentView::afterAnimation()
{
m_isAnimated=false;
emit currentPageChanged(mCurrentPage);
    mPixmapItemAnim->setVisible(false);

    if(mAnimation)
    delete mAnimation;

}

void DocumentView::animScrollbar(int current,int next)
{
   // qDebug()<<"animScrollbar";

    mAnimation = new QPropertyAnimation(verticalScrollBar(), "value");
    connect(mAnimation,SIGNAL(finished()),this,SLOT(afterAnimation()));
m_isAnimated=true;
    mAnimation->setDuration(300);
    mAnimation->setStartValue(current);
    mAnimation->setEndValue(next);
    mAnimation->start();

}
/*
QMap<QString,QString>DocumentView::docInfo()
{
    //-----------------PIXMAP------------------
    QImage image = imagePage(0,QSizeF(256.0,256.0));

      QPixmap pix= QPixmap::fromImage(image);
      QString name=QFileInfo(mFilePath).baseName();
     name.replace(" ","_");
  if(QFile::exists(thumbnails+"/"+name+".png")){
      QString n=name;
     int i=0;
      do{
           i++;
           name=n+QString::number(i);
      }while (QFile::exists(thumbnails+"/"+name+".png"));
  }
   pix.save(thumbnails+"/"+name+".png");
  //-----------------PIXMAP------------------

  //--------------------INFO-------------------
  QStringList tlist= mDocument->infoKeys();
  QString info;
  QString autors;
  foreach (QString s, tlist) {
      QString value=mDocument->info(s);
      if(s=="Author"){
          autors=value;
          if(!value.isEmpty())
              info+=s+" :"+value+"\n";
      }

      if(value.startsWith("D:"))
      {
          value = mDocument->date(s).toString();
      }

      if(s=="CreationDate"||s=="Title"||s=="Creator"
              ||s=="Producer"||s=="ModDate"||s=="Format")
          if(!value.isEmpty())
              info+=s+" :"+value+"\n";


  }

  info+="Pages :"+QString::number(mDocument->numPages());


  //--------------------INFO-------------------



      QMap<QString,QString>mMap;
      mMap.insert("Name",QFileInfo(mFilePath).baseName().remove("'"));
      mMap.insert("Author",autors);
      mMap.insert("Group","");
      mMap.insert("Icon",name+".png");
      mMap.insert("Comment","");
      mMap.insert("Properties",info.remove("'"));
      mMap.insert("Exec",mFilePath);

      return mMap;
}

*/


void DocumentView::setInvertColors(bool arg)
{

    ::PageItem::setInvertColors(arg);
    foreach (PageItem* page , mPages)
    {
        page->refresh();
    }
}

QSizeF DocumentView::curentPagePos()
{

    if(!mDocument)return QSizeF();

        qreal left = 0.0, top = 0.0;
    saveLeftAndTop(left, top);
     return QSizeF(left,top);

}

QStandardItemModel* DocumentView::thumbnailsModel()
{


    QStandardItemModel* thumbnailsModel = new QStandardItemModel();

 if(!mDocument)return thumbnailsModel;

    thumbnailsModel->invisibleRootItem()->insertColumns(0, 1);

    for(int index = 0; index < mPagesNumber; ++index)
    {
        QApplication::processEvents();

        QStandardItem* item = new QStandardItem();
        QString num=this->locale().toString(index+1);
        item->setText(num);
        item->setData(index,Qt::UserRole);
        //  item->setIcon(icon);
        thumbnailsModel->invisibleRootItem()->appendRow(item);

    }
    return thumbnailsModel;
}
 QStandardItemModel* DocumentView::outlineModel()
 {


     QStandardItemModel* contentsModel = new QStandardItemModel();
    if(!mDocument)return contentsModel;

  contentsModel->invisibleRootItem()->insertColumns(0, 0);

      QDomDocument* toc = mDocument->toc();

      if(toc != 0)
      {
          prepareOutline(toc->firstChild(), contentsModel->invisibleRootItem());

          delete toc;
      }else{
          for(int index = 0; index < mPagesNumber; ++index)
          {
              QApplication::processEvents();

              QStandardItem* item = new QStandardItem();
              QString num=this->locale().toString(index+1);

              item->setText(tr("Page ")+num);
              item->setData(index+1, Qt::UserRole);
              contentsModel->invisibleRootItem()->appendRow(item);

          }
      }
     return contentsModel;
 }

//_________________________________________________
QStandardItemModel* DocumentView::fontsModel()
{


 if(mFontsModel)
      return mFontsModel;

 mFontsModel = new QStandardItemModel();

 if(!mDocument)return mFontsModel;

     QList< Poppler::FontInfo > fonts = mDocument->fonts();

    mFontsModel->setRowCount(fonts.count());
    mFontsModel->setColumnCount(5);

    mFontsModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Type") << tr("Embedded") << tr("Subset") << tr("File"));

    for(int index = 0; index < fonts.count(); ++index)
    {
        Poppler::FontInfo& font = fonts[index];

        mFontsModel->setItem(index, 0, new QStandardItem(font.name()));
        mFontsModel->setItem(index, 1, new QStandardItem(font.typeName()));
        mFontsModel->setItem(index, 2, new QStandardItem(font.isEmbedded() ? tr("Yes") : tr("No")));
        mFontsModel->setItem(index, 3, new QStandardItem(font.isSubset() ? tr("Yes") : tr("No")));
        mFontsModel->setItem(index, 4, new QStandardItem(font.file()));
    }

    return mFontsModel;
}

QStandardItemModel* DocumentView::propertiesModel()
{


    QStandardItemModel* propertiesModel = new QStandardItemModel();

    if(!mDocument)return propertiesModel;

    QStringList keys = mDocument->infoKeys();

    propertiesModel->setRowCount(keys.count());
    propertiesModel->setColumnCount(2);
    propertiesModel->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Value") );

    propertiesModel->setItem(0, 0, new QStandardItem(tr("Pages")));
    propertiesModel->setItem(0, 1, new QStandardItem(QString::number(mPagesNumber)));

    propertiesModel->setItem(1, 0, new QStandardItem(tr("File")));
    propertiesModel->setItem(1, 1, new QStandardItem(mFilePath));

    for(int index = 2; index < keys.count(); ++index)
    {
        QString key = keys.at(index);
        QString value = mDocument->info(key);


        if(value.startsWith("D:"))
        {
            value = mDocument->date(key).toString();
        }

        key.replace("CreationDate",tr("Created"));
        key.replace("Title",tr("Title"));
        key.replace("Author",tr("Author"));
        key.replace("Creator",tr("Creator"));
        key.replace("Producer",tr("Producer"));
        key.replace("ModDate",tr("Modified"));
        key.replace("Format",tr("Format"));
        key.replace("Pages",tr("Pages"));

        propertiesModel->setItem(index, 0, new QStandardItem(key));
        propertiesModel->setItem(index, 1, new QStandardItem(value));
    }
    return propertiesModel;
}
