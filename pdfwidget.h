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

#ifndef PDFWIDGET_H
#define PDFWIDGET_H

#include <QLabel>
#include <QRectF>
#include <QTreeWidget>

#if QT_VERSION >= 0x050000
#include <poppler-qt5.h>
#else
#include <poppler-qt4.h>
#endif

#include <QScrollArea>
#include<QScrollBar>
#include <QMenu>
//class QRubberBand;
class Label: public QLabel
{
    Q_OBJECT

public:
    Label(QWidget *parent = 0);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
 //   void pageChanged(int currentPage);
  //  void textSelected(const QString &text);
    void posChanged(int x,int y);


private:
       QPoint dragPosition;
};

class PdfWidget : public QScrollArea
{
    Q_OBJECT

protected:
    void resizeEvent ( QResizeEvent *  ) {fitInwidthPdf(m_fitInwidth);}

public:
    PdfWidget(QWidget *parent = 0);
    ~PdfWidget();
    Poppler::Document *document();
signals:
    void pageChanged(int currentPage);
    void textSelected(const QString &text);
 void fitWitdhChanged(bool);

public slots:
    int pagesCount();
    QWidget *PdfWidgetScroll(){return this;}
      int pageCurent(){return currentPage+1;}
      void setListActions(QList<QAction *> list){mListAction=list;}

      QStringList infoKeys()const {
          if(doc)return doc->infoKeys();
          return QStringList();
          }
      QImage pixmap(){
            if(doc)
                return label->pixmap()->toImage();
            return QImage();
      }
      QString info( const QString & data ) const{
          if(doc)return doc->info(data);
          return QString();
    }

    QRectF searchForwards(const QString &text);
    bool setDocument(const QString &filePath);
    void closeDoc(){
        if(doc){
            delete doc;
            doc=0;
        }
    }
    void setCostmColor(bool costum,const QString &color){
        isCostmColor=costum;

        costumColor=color;
        if(doc){
            if(isCostmColor)
                doc->setPaperColor(QColor(costumColor));
            else
                doc->setPaperColor(QColor(originColor));
            showPage(currentPage+1);
        }
    }

    void setPage(int page = -1, QRectF rectF=QRectF(0,0,0,0));
      void loadToc(QTreeWidget *view);
void zoomOut();
void zoomIn();
void zoomOriginal();

    //#ifndef QT_NO_WHEELEVENT
    //        /*virtual*/ void wheelEvent(QWheelEvent *event);
    // #endif // QT_NO_WHEELEVENT



   // void posChanged(int x,int y);

private slots:
      void setEnsureVisible(QRectF rectF);
    void setScale(qreal scale);
  QRectF searchBackwards(const QString &text);
    void fitInwidthPdf(bool fit);
    void selectedText(const QRectF &rect);
    void showPage(int page = -1);
    void searchToc(QDomNode node,QTreeWidgetItem *item);
    void scrollByMouse(int x,int y);
    void customContextMenu();
private:
    QMatrix matrix() const;
    Poppler::Document *doc;
    int currentPage;
QList<QAction *> mListAction;
   // QRubberBand *rubberBand;
    QRectF searchLocation;
    qreal scaleFactor;
    bool isCostmColor;
    QString costumColor;
    QString originColor;
    Label *label;
   // QScrollArea   *scrollArea;
       qreal scalPdf;
       qreal pdfWidth;
       bool m_fitInwidth;
};

#endif
