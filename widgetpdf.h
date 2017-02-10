/* Copyright © Abouzakaria
 * (c)GPL2 (c)GPL3
 *
 * This file is part of booksorg.
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

#ifndef WIDGETPDF_H
#define WIDGETPDF_H
#include "documentview.h"
#include "itemdelegate.h"
#include "searchtools.h"
#include <QWidget>
#include <QToolBar>
#include <QActionGroup>
#include <QSpinBox>

namespace Ui {
class WidgetPdf;
}

class WidgetPdf : public QWidget
{
    Q_OBJECT
    
public:

     //!
    explicit WidgetPdf(QWidget *parent = 0);

     //!
    ~WidgetPdf();
    
     //!
    bool openDoc(const QString &filePath, int page);

     //!
    QList<QAction*>listEdit(){return mListEdit;}

     //!
//    QList<QAction*>listView(){return mListView;}

     //!
//    QMap<QString,QString>docInfo(){return mDocumentView->docInfo();}

     //!
    QString filePath(){return mDocumentView->filePath();}

     //!

    int numPages(){return mDocumentView->numberOfPages();}
    int curPage(){return mDocumentView->currentPage();}
    void jumpToPage(int page,qreal left,qreal top);

    void docToggleContentVisible(bool arg);
    bool isContentVisible();

    void docEmitFirstPage();
    void docEmitPreviousPage();
    void docEmitNextPage();
    void docEmitLastPage();
    void docEmitAddBookmark();
    void docRefresh();
    void docEmitZoomIn();
    void docEmitZoomOut();

    void docEmitSetRubberBand(bool arg);
     bool isRubberBand();

     void docEmitSetInvertColors(bool arg);
  bool isInvertColors(){return mInvertColors;}

     void docSetPage(int arg);

    void docSetContinousMode(bool arg);
     bool isContinousMode(){return mDocumentView->continousMode();}

    void docSetFiteWith(int arg);
    int docFiteWith(){return mFitSize;}

    void docSetLayoutMode(int arg);
 int doctLayoutMode(){return mDocumentView->layoutMode();}

    void showHideSearch();

    QStandardItemModel* fontsModel();

    QStandardItemModel* propertiesModel();

    void saveLogoPixmap(const QString &file);
signals:

    //!
   void curPageChanged(int page);

     //!
    void pageChanged(const QString &file,int page);

     //!
    void  docChanged(const QString);

     //!
    void addNewBookmark(const QString& ,int ,qreal ,qreal ,const QString &str);

public slots:

     //!
    void addBookmark(const QString &str=QString());

private slots:

     //!
    void goSearch(const QString &text);


     //!
 //   void setFitSize(QAction* act);

     //!
    void setLayoutMode(QAction*act);

     //!
    void slotPageChanged(int page);

     //!
    bool findIndex(QModelIndex index, int page);


     //!
   // void swithContents(int i);

     //!
    void on_treeViewPdf_clicked(const QModelIndex &index);

     //!
    void on_listViewPdf_clicked(const QModelIndex &index);

     //!
    void on_listViewPdf_activated(const QModelIndex &index);

     //!
    void on_treeViewPdf_activated(const QModelIndex &index);

     //!
  //  void customMenuBookmark(QPoint);

    //!
//    void saveBookmarkSettings();

     //!
 //   void loadBookmarkSettings();

    //!
  //  void on_treeWidgetBookmark_itemActivated(QTreeWidgetItem *item, int);

     //!
  //  void removeBookmark();

     //!
  //  void renameBookmark();

     //!
  //  void toggleRubberBand(bool arg);

    void on_tabWidget_tabBarClicked(int index);

    void on_tabWidget_currentChanged(int index);

void showHideContent();
Qt::ArrowType arrow(bool opened);
private:

    Ui::WidgetPdf *ui;

//    QToolBar *mToolBarDoc;
 //   QToolBar *mToolBarContent;

  QAction *actAddBookmark;
    QList<QAction*>mListEdit;

 //   QActionGroup  *alignactContent;

   // QAction *actTree;

    DocumentView *mDocumentView;

 //   bool mThumbnailsCharged;
    ItemDelegate *mItemDelegate;
    SearchTools *mSearchTools;

     int mFitSize;
     bool mInvertColors;
};

#endif // WIDGETPDF_H
