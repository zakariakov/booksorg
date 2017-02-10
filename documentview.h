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
#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

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

class SearchThread;

class DocumentView : public QGraphicsView
{
    Q_OBJECT

public:
    //! تكوين المشهد
    explicit DocumentView(QWidget* parent = 0);
    ~DocumentView();

    //! دليل الملف
    const QString& filePath() const{  return mFilePath; }

    //! عدد الصفخات
    int numberOfPages() const {return mPagesNumber;}
    //! الصفحة الحالية
    int currentPage() const{return mCurrentPage;}
    //! موضه الصفحة الخالية
    QSizeF curentPagePos();

    // العرض
    //! عرض التتابع
    bool continousMode() const{ return mContinuousMode; }

    //! عرض حسب الصفحات
    enum LayoutMode
    {
        SinglePageMode = 0,
        TwoPagesMode = 1,
        TwoPagesWithCoverPageMode = 2,
        MultiplePagesMode = 3,
        NumberOfLayoutModes = 4
    };
    //! عرض حسب الصفحات
    LayoutMode layoutMode() const{ return mLayoutMode; }
    //!عرض حسب الصفحات
    void setLayoutMode(LayoutMode layoutMode);

    //! عرض حسب الحجم
    enum ScaleMode
    {
        ScaleFactor = 0,
        FitToPageWidth = 1,
        FitToPageSize = 2,
        NumberOfScaleModes = 3
    };
    //! عرض حسب الحجم
    ScaleMode scaleMode() const{ return mScaleMode;  }
    //!     عرض حسب الحجم
    void setScaleMode(ScaleMode scaleMode);
    //!
    qreal scaleFactor() const{return mScaleFactor; }
    //!
    void setScaleFactor(qreal scaleFactor);

    // تدوير الصفحات
    //! قلب الصفحات الحالي
    Poppler::Page::Rotation rotation() const{return mRotation;}
    //! تطبيق القلب
    void setRotation(Poppler::Page::Rotation rotation);


    PageItem::RubberBandMode rubberBandMode() const{return mRubberBandMode; }
    //!
    void setRubberBandMode(PageItem::RubberBandMode rubberBandMode);
    //!
    bool searchWasCanceled() const;

    //!
    int searchProgress() const;

    //!
    bool highlightAll() const{ return mHighlightAll;}
    //!
    void setHighlightAll(bool highlightAll);
    //!

    QStandardItemModel* outlineModel() ;/*const{return mContentsModel;}*/


    QStandardItemModel* thumbnailsModel(); /*const{return mThumbnailsModel;}*/

    QStandardItemModel* fontsModel();

    QStandardItemModel* propertiesModel();

    //!
    bool isValid(){return mDocument;}
bool isRubberBandMode(){return isRubberPrivat;}


signals:
    //!
    void filePathChanged(const QString& filePath);
    void numberOfPagesChanged(int numberOfPages);
    void currentPageChanged(int currentPage, bool returnTo = false);
    //!
    void continousModeChanged(bool continousMode);
    void layoutModeChanged(DocumentView::LayoutMode layoutMode);
    void scaleModeChanged(DocumentView::ScaleMode scaleMode);
    void scaleFactorChanged(qreal scaleFactor);
    void rotationChanged(Poppler::Page::Rotation rotation);
    //!
    void highlightAllChanged(bool highlightAll);
    void rubberBandModeChanged(PageItem::RubberBandMode rubberBandMode);
    //!
    //  void searchProgressed(int progress);
    void searchFinished();
    void searchCanceled();
    void addBookmark(const QString&);
    //  void bookmarkAdded(int page,qreal top,qreal left);

public slots:
    //!
    void show();
    //!
    void setContinousMode(bool continousMode);
    //!
    PageItem* page(int index){ return  mPages.at(index);}
     Poppler::Page* pageDoc(int index);
     //!
  //  void prepareThumbnails();
    //!
    QImage imagePage(int index, QSizeF sizeF);
    //!
    void setInvertColors(bool arg);
    //!
//    QMap<QString,QString>docInfo();
    //!
    bool open(const QString& filePath);
    //!
    bool refresh();
    //!
    void setListEdit(QList<QAction*>list){mListEdit=list;}
    //!
    void setRubberBand(bool r);

    // التنقل
    //!
    void previousPage();
    //!
    void nextPage();
    //!
    void firstPage();
    //!
    void lastPage();
    //!
    void jumpToPage(int page, bool returnTo = true, qreal changeLeft = 0.0, qreal changeTop = 0.0,bool anim=true);
    //!
    void jumpToHighlight(const QRectF& highlight);
    //
    //!
    void startSearch(const QString& text, bool matchCase = true);
    //!
    void cancelSearch();
    //!
    void findPrevious();
    //!
    void findNext();

    //العرض
    //!
    void zoomIn();
    //!
    void zoomOut();
    //!
    void originalSize();
    //!
    void fitPageWith();
    //!
    void fitPageSize();
    //!
    void rotateLeft();
    //!
    void rotateRight();

    //



protected slots:

    void on_verticalScrollBar_valueChanged(int value);

    void on_searchThread_resultsReady(int index, QList< QRectF > results);

    void on_pages_linkClicked(int page, qreal left, qreal top);

    void on_pages_linkClicked(const QString& url);

    void on_pages_rubberBandFinished();

protected:

    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);

private:



    static Qt::KeyboardModifiers s_zoomModifiers;
    static Qt::KeyboardModifiers s_rotateModifiers;
    static Qt::KeyboardModifiers s_horizontalModifiers;

    //!
    bool isRubberPrivat;
    //!
    Poppler::Document* mDocument;
    //!
    QString mFilePath;
    //!
    int mPagesNumber;
    //!
    int mCurrentPage;
    //!
    int mPrevPage;
    //!
    QStack< int > m_returnToPage;
    //!
    QStack< qreal > m_returnToLeft;
    //!
    QStack< qreal > m_returnToTop;
    //!
    bool mContinuousMode;
    //!
    LayoutMode mLayoutMode;
    //!
    ScaleMode mScaleMode;
    //!
    qreal mScaleFactor;
    //!
    Poppler::Page::Rotation mRotation;
    //!
    bool mHighlightAll;
    //!
    PageItem::RubberBandMode mRubberBandMode;
    //!
    QGraphicsScene* mScene;
    //!
    QVector< PageItem* > mPages;
    //!
    QMap< qreal, int > mHeightToIndex;
    //!
    QList<QAction*>mListEdit;
    //!
    QGraphicsRectItem* mHighlight;
    //!
    //QStandardItemModel *mThumbnailsModel;
    //!
   // QStandardItemModel* mContentsModel;
    //!
    QPropertyAnimation *mAnimation;
    //!
    QGraphicsPixmapItem *mPixmapItemAnim;
    //!
    bool m_isAnimated;

    // البحث
    //!
    QMultiMap< int, QRectF > mResults;
    //!
    QMultiMap< int, QRectF >::iterator mCurrentResult;
    //!
    SearchThread* mSearchThread;
 QStandardItemModel* mFontsModel ;

// QHash< int,Thumbnail* > mThumbnails;

private slots:

    // التهيئة
    //! تهيئة الوثيقة
    void prepareDocument(Poppler::Document* document);
    //! تهيئة الصفحات
    void preparePages();
    //! تهيئة الفهرسة
    //void prepareOutline();
    //! تهيئة ابناء الفهارس
    void prepareOutline(const QDomNode& node, QStandardItem* parent);
    //!تهيئة المشهد
    void prepareScene();
    //! تهيئة العرض
    void prepareView(qreal changeLeft = 0.0, qreal changeTop = 0.0, bool anim=false);
    //!تهيئة تحديد البحث
    void prepareHighlight();

    //وضع الحركة
    //! الحركة في حالة عرض الصفحة
    void animPage(PageItem *page, bool top);
    //!الحركة في حالة تتابع الصفخات
    void animScrollbar(int current,int next);
    //! بعد انتهاء الحركة
    void afterAnimation();

    //! جلب الصفحة الحالية باعتبار العرض
    int currentPageForPage(int page) const;
    //!
    int leftIndexForIndex(int index) const;
    //!
    int rightIndexForIndex(int index) const;
    //!
    void saveLeftAndTop(qreal& left, qreal& top) const;


};

#endif // DOCUMENTVIEW_H
