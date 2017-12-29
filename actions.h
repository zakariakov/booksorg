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

#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>
#include <QAction>
#include <QWidgetAction>
#include <QSpinBox>
#include<QMenu>
class Actions : public QObject
{
    Q_OBJECT
public:
    explicit Actions(QLocale lc,QObject *parent = 0);

    //Books______________________________________________
    QList<QAction *>listCatBooks();/*{return mListcatBooks;}*/
    QList<QAction *>listViewBooks();/*{return mListViewBooks;}*/
    QList<QAction *>listEditBooks();/*{return mListEditBooks;}*/
    QList<QAction *>listStandardToolBarBooks();
    QList<QAction *>listToolBarBooks();
    QList<QAction *>listAllBooksActions();

    //Pdfs________________________________________________
    QList<QAction *>listEditPdf();/*{return mListEditPdf;}*/
    QList<QAction *>listViewPdf();/*{return mListViewPdf;}*/
    QList<QAction *>listStandardToolBarPdf();
    QList<QAction *>listToolBarPdf();
    QList<QAction *>listAllPdfActions();
//
    QAction *goHome(){return mActRootPath;}
    QAction *editBookInfo(){return mActEditBookInfo;}
signals:

    //Books______________________________________________SIGNAL
    void emitRootPath();
    void emitEditBookOrGroup();
    void emitDeletSelectedIndexes();
    void viewModeChanged(int);
    void categoryChanged(int);

    //Pdfs________________________________________________SIGNAL
    //  void pdfToggleContentVisible(bool);
    void pdfEmitFirstPage();
    void pdfEmitPreviousPage();
    void currentPageChanged(int);
    void pdfEmitNextPage();
    void pdfEmitLastPage();
    void pdfEmitAddBookmark();
    void pdfRefresh();
    void pdfEmitZoomIn();
    void pdfEmitZoomOut();
    void pdfEmitSetRubberBand(bool);
    void pdfEmitSetInvertColors(bool);
    void pdfSetPage(int);
    void pdfSetContinousMode(bool);
    void pdfSetFiteWith(int );
    void pdfSetLayoutMode(int );
    void pdfEditBook();

public slots:

    void setSpinPages(int numPages,int page);
    void setRubberBandAct(bool arg);
    //  void setPdfContentVisibleAct(bool arg);
    void setFitSizeAct(int fit);
    void setLayoutModeAct(int layout);
    void setContinousModeAct(bool arg);
    void setInvertcolorsAct(bool arg);

private slots:

    //Books______________________________________________
    void setupBooksActions();
    void toggleView(QAction* action);
    void toggleRootPath(QAction// QAction *mActDelete;
                        // QAction *mActMove;
                      * action);

    //Pdfs________________________________________________
    void setupPdfsActions();
    void pdfGoPage();
    void disableFite();
    void setFitSizeAct(QAction* act);

    void setLayoutModeAct(QAction* act);

private:
    QLocale mlocale;
    //Books______________________________________________
    enum CateGory{
        CAT_GROUP,
        CAT_AUTHOR,
        CAT_FAVORIT,
        CAT_SEARCH
    };

    enum ViewMode{
        IconView,
        DetailView
    };

    QAction *mActDelete;
    QAction *mActEditBook;
    QAction *mActRootPath;
    QAction *mActGroopPath;
    QAction *mActAutorPath;
    QAction *mActFavoPath;
    QAction *actIconView;
    QAction *actDetailView;
    QAction *actViewBook;
    QMenu  *menuViewBook;
    QAction *actPath;
    QMenu *menuPath;
    QActionGroup  *alignViewMode;
    QActionGroup  *alignRootPath;
    QAction *sepBook;
    QAction *sepBook2;
    QAction *sepBook3;

    //QList<QAction*>mListcatBooks;

   // QList<QAction*>mListViewBooks;

   // QList<QAction*>mListEditBooks;
   // QList<QAction*>mListToolBarBooks;


    //Pdfs________________________________________________
    QAction *mActShowHide;
    QAction *mActFirst;
    QAction *mActPrev;
    QAction *mActNext;
    QAction *mActLast;
    QWidgetAction *mActSpin;
    QSpinBox *mSpinBox;
    QAction *mActAddBookmark;
    QAction *mActRefresh;
    QAction *mActZoomIn;
    QAction *mActZoomOut;
    QAction *mActEditBookInfo;


    QActionGroup *mAlignActionsFite;
    QAction *mActOriginalSize;
    QAction *mActZoomFit;
    QAction *mActZoomBest;
    QAction *mActSizeFite;
    QMenu *menuSizeFite;

    QAction *mActContinous;
    QAction *mActRubberBand;
    QAction *mActInvertcolors;
    QActionGroup *alignLayoutMode;
    QAction *mActSinglePage;
    QAction *mActTwoPages;


   // QList<QAction*>mListEditPdf;
   // QList<QAction*>mListViewPdf;
    //QList<QAction*>mListToolBarPdf;

    QAction *sepPdf;
    QAction *sepPdf2;
    QAction *sepPdf3;
    QAction *sepPdf4;
};

#endif // ACTIONS_H
