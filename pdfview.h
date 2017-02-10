#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <QGraphicsView>

#include <QtCore>
#include <QtXml>
#include <QtGui>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)

#include <QtWidgets>
#include <QtPrintSupport>
#include <poppler-qt5.h>

#else
#include <poppler-qt4.h>
#endif // QT_VERSION
#include "pageitem.h"
#include "searchthread.h"


class PdfView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PdfView(QWidget *parent = 0);

signals:

public slots:

private slots:

    //!animation
    void animPage(PageItem *page, bool top);
    void animScrollbar(int current,int next);
     void afterAnimation();

     //
     void prepareView(qreal changeLeft = 0.0, qreal changeTop = 0.0, bool anim=false);

private:
    //!وثيقة بي دي اف
    Poppler::Document* mDocument;
    //! تحديد البحث
    QGraphicsRectItem* mHighlight;
    //! مشهد الحاوي للصفحات
    QGraphicsScene* mGraphicsScene;
    //!  موديول الفهرسة
    QStandardItemModel* mContentsModel;
    //! موديول المصغرات
    QStandardItemModel   *mThumbnailsModel ;
    //! الصورة البديلة في حالة الحركة
    QGraphicsPixmapItem *mPixmapItem;
    //! كلاس البحث
    SearchThread *mSearchThread ;
    //!
    QVector< PageItem* > mPages;

    //!
     bool mContinuousMode;
    //!
     QMultiMap< int, QRectF > mResults;
       //!
     QMultiMap< int, QRectF >::iterator mCurrentResult;

    //!
     QString mFilePath;

     //!
     int mNumberPages;

     //!
     int mCurrentPage;

     //!
     int mPrevPage;
    //!
     QPropertyAnimation *m_animation;

    //!

    //!
};

#endif // PDFVIEW_H
