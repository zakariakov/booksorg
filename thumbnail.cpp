#include "thumbnail.h"
#include <QPaintDevice>
Thumbnail::Thumbnail(Poppler::Page *page, QModelIndex index, QObject *parent)
: QObject(parent),
m_physicalDpiX(72),
m_physicalDpiY(72),
m_scaleFactor(1.0),
m_rotation(Poppler::Page::Rotate0)
{
    m_page = page;
    m_index=index;

    QSizeF pageSize = page->pageSizeF();//150
    m_scaleFactor = qMin( 150/pageSize.width() ,   150/pageSize.height());

    m_render = new QFutureWatcher< void >(this);

    connect(m_render, SIGNAL(finished()),
            SLOT(on_render_finished()));

    connect(this, SIGNAL(imageReady(QImage))
            , SLOT(on_imageReady(QImage)));

    startRender();

}

 Thumbnail::~Thumbnail()
 {

 }

 void Thumbnail::on_render_finished()
 {
     imageFiniched(m_index);
 }

 void Thumbnail::on_imageReady(QImage image)
 {

         if(!m_render->isCanceled())
         {
             m_image = image;
         }

 }

 void Thumbnail::startRender()
 {
     if(!m_render->isRunning())
     {
         m_render->setFuture(QtConcurrent::run(this, &Thumbnail::render));
     }
 }

 void Thumbnail::cancelRender()
 {
     m_render->cancel();
     m_image = QImage();
 }

 //____________________________________________________________________
 void Thumbnail::render()
 {


     if(m_render->isCanceled())
     {
         return;
     }

     QImage  image = m_page->renderToImage(m_scaleFactor * m_physicalDpiX,
                                           m_scaleFactor * m_physicalDpiX,
                                           -1, -1, -1, -1, m_rotation);

     if(m_render->isCanceled())
     {
         return;
     }

     emit imageReady(image);
 }
