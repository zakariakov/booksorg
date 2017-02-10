#ifndef THUMBNAIL_H
#define THUMBNAIL_H
#include <QtCore>
#include <QtGui>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <poppler-qt5.h>

#include <QtWidgets>
#include <QtConcurrent>
#else

#include <poppler-qt4.h>
#endif // QT_VERSION



#include <QImage>
#include <QFutureWatcher>
class Thumbnail : public QObject
{
    Q_OBJECT
public:
    Thumbnail(Poppler::Page* page, QModelIndex index, QObject *parent = 0);
 ~Thumbnail();
    QImage image(){return m_image;}

//    QRectF boundingRect() const{}
//    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*){}

signals:
    void imageReady(QImage image);
void imageFiniched(QModelIndex index);
protected slots:

    void on_imageReady(QImage image);
void on_render_finished();
public slots:

    void startRender();
    void cancelRender();

private:
     QFutureWatcher< void >* m_render;

     int m_physicalDpiX;
     int m_physicalDpiY;
QModelIndex m_index;
     qreal m_scaleFactor;
     Poppler::Page::Rotation m_rotation;

     QTransform m_transform;
     QTransform m_normalizedTransform;
     QRectF m_boundingRect;
     Poppler::Page* m_page;

     QImage m_image;

        void render();

};

#endif // THUMBNAIL_H
