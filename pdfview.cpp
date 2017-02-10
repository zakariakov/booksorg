#include "pdfview.h"

#define thumbnails QDir::homePath()+"/."+QApplication::applicationName()+"/thumbnails"
#define SPACING 5.0
#define MINSCALE 0.1
#define MAXSCALE 10.0
#define ZOOMBY 0.1
#define HIGHL_DURATION 5000
#define PAGES_PERROW 3


PdfView::PdfView(QWidget *parent) :
    QGraphicsView(parent)
{
    mGraphicsScene = new QGraphicsScene(this);

    mContentsModel = new QStandardItemModel(this);

    mThumbnailsModel = new QStandardItemModel(this);

    setScene(mGraphicsScene);

    setDragMode(QGraphicsView::ScrollHandDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setAcceptDrops(false);

    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(on_verticalScrollBar_valueChanged(int)));

    // التحديد

    mHighlight = new QGraphicsRectItem();
    mPixmapItem=new QGraphicsPixmapItem;
    mPixmapItem->setVisible(false);
    mHighlight->setVisible(false);
    mGraphicsScene->addItem(mPixmapItem);
    mPixmapItem->setZValue(0);
    mGraphicsScene->addItem(mHighlight);

    QColor highlightColor = QApplication::palette().color(QPalette::Highlight);

    highlightColor.setAlpha(100);
    mHighlight->setBrush(QBrush(highlightColor));

    highlightColor.setAlpha(255);
    mHighlight->setPen(QPen(highlightColor));

    // البحث

    mSearchThread = new SearchThread(this);

    connect(mSearchThread, SIGNAL(resultsReady(int,QList<QRectF>)), SLOT(on_searchThread_resultsReady(int,QList<QRectF>)));
    connect(mSearchThread, SIGNAL(progressed(int)), SIGNAL(searchProgressed(int)));
    connect(mSearchThread, SIGNAL(finished()), SIGNAL(searchFinished()));
    connect(mSearchThread, SIGNAL(canceled()), SIGNAL(searchCanceled()));


}

//___________________________________________________________
void PdfView::prepareView(qreal changeLeft, qreal changeTop,bool anim)
{
    qreal left = m_pagesScene->sceneRect().left();
    qreal top = m_pagesScene->sceneRect().top();
    qreal width = m_pagesScene->sceneRect().width();
    qreal height = m_pagesScene->sceneRect().height();

    int horizontalValue = 0;
    int verticalValue = 0;

    //qDebug()<<"m_currentPage"<<m_currentPage;

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
                if(m_layoutMode==SinglePageMode&&anim){

                    if(mPrevPage>-1 && mPrevPage-1<mPages.count()){
                        PageItem* curpage = mPages.at(mPrevPage-1);
                        if(curpage){
                            m_PixmapItem->setPixmap(QPixmap::fromImage(curpage->image()));
                           m_PixmapItem->setVisible(true);
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
                m_highlight->setPos(page->pos());
                m_highlight->setTransform(page->transform());

                page->stackBefore(m_highlight);
            }
        }
    }

    setSceneRect(left, top, width, height);
    horizontalScrollBar()->setValue(horizontalValue);
    //verticalScrollBar()->setValue(verticalValue);
    if(mContinuousMode&&anim)
        animScrollbar(verticalScrollBar()->value(),verticalValue);
    else
        verticalScrollBar()->setValue(verticalValue);

    viewport()->update();
}

//_________________________________________________
void PdfView::animPage( PageItem* page,bool top)
{


    QRectF boundingRect = page->boundingRect().translated(page->pos());
    mPixmapItem->setPos(QPoint(boundingRect.x(),boundingRect.y()));

    QPoint start;
    if(top)
        start=QPoint(boundingRect.x(),boundingRect.top()+boundingRect.height());
    else
        start=QPoint(boundingRect.x(),boundingRect.top()-boundingRect.height());

    m_animation = new QPropertyAnimation(page, "pos");
    connect(m_animation,SIGNAL(finished()),this,SLOT(afterAnimation()));

    m_animation->setDuration(300);
    m_animation->setStartValue(start);

    m_animation->setEndValue(QPoint(boundingRect.x(),boundingRect.y()));

    m_animation->start();

}

//___________________________________________________
void PdfView::afterAnimation()
{
m_isAnimated=false;
emit currentPageChanged(m_currentPage);
    mPixmapItem->setVisible(false);

    if(m_animation)
    delete m_animation;

}

//___________________________________________________
void PdfView::animScrollbar(int current,int next)
{
   // qDebug()<<"animScrollbar";
    m_animation = new QPropertyAnimation(verticalScrollBar(), "value");
    connect(m_animation,SIGNAL(finished()),this,SLOT(afterAnimation()));
m_isAnimated=true;
    m_animation->setDuration(300);
    m_animation->setStartValue(current);
    m_animation->setEndValue(next);
    m_animation->start();

}
