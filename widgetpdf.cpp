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

#include "widgetpdf.h"
#include "ui_widgetpdf.h"
#include "itemdelegate.h"
#include "eicon.h"

#include <QSettings>
#include <QMessageBox>

//___________________________________________________________
WidgetPdf::WidgetPdf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPdf)
{
    ui->setupUi(this);

if(isRightToLeft())
    ui->tabWidget->setTabPosition(QTabWidget::East);
else
      ui->tabWidget->setTabPosition(QTabWidget::West);

    ui->widgetContents->hide();
  //  mThumbnailsCharged=true;

connect(ui->toolButtonShowHide,SIGNAL(clicked()),this,SLOT(showHideContent()));
ui->toolButtonShowHide->setShortcut(QKeySequence("Ctrl+L"));
ui->toolButtonShowHide->setToolTip(tr("Show/Hide Content")+" Ctrl+L");
ui->toolButtonShowHide->setStatusTip(tr("Show/Hide Content")+" Ctrl+L");
  ui-> toolButtonShowHide->setArrowType(arrow(false));
    mDocumentView=new DocumentView;
    ui->vLayoutDoc->addWidget(mDocumentView);

    mSearchTools=new SearchTools(this);
    ui->vLayoutDoc->addWidget(mSearchTools);
    mSearchTools->setVisible(false);

    mItemDelegate=new ItemDelegate(this);
    mItemDelegate->setView(mDocumentView);

    ui->listViewPdf->setItemDelegate(mItemDelegate);
    ui->treeViewPdf->setHeaderHidden(true);



    mDocumentView->setHighlightAll(true);

    connect(mSearchTools,SIGNAL(searchPressed(QString)),this
            ,SLOT(goSearch(QString)));
    connect(mSearchTools,SIGNAL(searchNext()),mDocumentView
            ,SLOT(findNext()));
    connect(mSearchTools,SIGNAL(searchPreviuos()),mDocumentView
            ,SLOT(findPrevious()));
    connect(mSearchTools,SIGNAL(searchCleared()),mDocumentView
            ,SLOT(cancelSearch()));
    connect(mDocumentView,SIGNAL(currentPageChanged(int)),this
            ,SLOT(slotPageChanged(int)));
    connect(mDocumentView,SIGNAL(addBookmark(QString)),this
            ,SLOT(addBookmark(QString)));


}


//_______________________________________________________
WidgetPdf::~WidgetPdf()
{

    if(!mDocumentView->filePath().isEmpty())
        emit pageChanged(mDocumentView->filePath(),mDocumentView->currentPage());

    delete mItemDelegate;
    delete mDocumentView;

    delete ui;
}


//_______________________________________________________
bool WidgetPdf::openDoc(const QString &filePath,int page )
{

    if(!mDocumentView->filePath().isEmpty())
        emit pageChanged(mDocumentView->filePath(),mDocumentView->currentPage());

    if( mDocumentView->open(filePath))
    {
        QSettings setting;
        setting.beginGroup(filePath);
        //FitSize
        int mFitSize=  setting.value("FitSize",0).toInt();
        docSetFiteWith(mFitSize);

        //ContinousMode
        bool continueMode=setting.value("ContinousMode",true).toBool();
        docSetContinousMode(continueMode);

        //IvertColors
        bool invert=setting.value("IvertColors",false).toBool();
        docEmitSetInvertColors(invert);

        //LayoutMode
        int lmode=setting.value("LayoutMode",0).toInt();
        docSetLayoutMode(lmode);

        setting.endGroup();

       // mItemDelegate->clear();



        mDocumentView->setFocus();
      //  mThumbnailsCharged=false;
     mDocumentView->jumpToPage(page, false,0,0,false);

     ui->treeViewPdf->setModel(mDocumentView->outlineModel());
     ui->listViewPdf->setModel(mDocumentView->thumbnailsModel());


        return true;

    }
    else
    {
        QMessageBox::warning(this, tr("Warning")
                             , tr("Couldn't open file  '%1'.").arg(filePath));
    }
    return false;
}

//_________________________________________________________________
//void WidgetPdf::swithContents(int i)
//{

    // ui->tabWidget->setCurrentIndex(i);
//    if(!mThumbnailsCharged && i==1){
//        mDocumentView->prepareThumbnails();
//        mThumbnailsCharged=true;
//    }

//}

//________________________________________________________________
void WidgetPdf::on_treeViewPdf_clicked(const QModelIndex &index)
{

    bool ok = false;
    int page = ui->treeViewPdf->model()->data(index, Qt::UserRole).toInt(&ok);
    qreal left =ui->treeViewPdf->model()->data(index, Qt::UserRole + 2).toReal();
    qreal top = ui->treeViewPdf->model()->data(index, Qt::UserRole + 3).toReal();

    if(ok)
        mDocumentView->jumpToPage(page, true, left, top);

    ui->treeViewPdf->setCurrentIndex(index);
}

//________________________________________________________________
void WidgetPdf::on_listViewPdf_clicked(const QModelIndex &index)
{

    bool ok = false;
    int page=ui->listViewPdf->model()->data(index,Qt::UserRole).toInt(&ok);
qDebug()<<"WidgetPdf::on_listViewPdf_clicked page:"<<page;
    if(ok)
        mDocumentView->jumpToPage(page+1);


}

//________________________________________________________________
void WidgetPdf::on_listViewPdf_activated(const QModelIndex &index)
{
    on_listViewPdf_clicked(index);
}

//________________________________________________________________
void WidgetPdf::on_treeViewPdf_activated(const QModelIndex &index)
{
    on_treeViewPdf_clicked(index);
}

//________________________________________________________Search
void WidgetPdf::goSearch(const QString &text)
{
    //TODO:add searsh derection for qt5
    if(!text.isEmpty())
    {
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
        qDebug()<<" WidgetPdf::goSearch local:"<<QGuiApplication::inputMethod()->locale();

        if(mSearchTools->inputDirection()==Qt::LeftToRight)
            mDocumentView->startSearch(text,mSearchTools->isMachCase());
        else{
            QString str;
            for(int i=text.length();i>-1;i--){
                str+=text.mid(i,1);
                //qDebug()<<str;
            }
            mDocumentView->startSearch(str,mSearchTools->isMachCase());
        }
#else

        if(QApplication::keyboardInputDirection()==Qt::RightToLeft){

            QString str;
            for(int i=text.length();i>-1;i--)
            {
                str+=text.mid(i,1);
              //  qDebug()<<str;
            }
            mDocumentView->startSearch(str,mSearchTools->isMachCase());

        }else{

            mDocumentView->startSearch(text,mSearchTools->isMachCase());
        }
#endif


    }
}

//________________________________________________________Search
void WidgetPdf::showHideSearch()
{

    if(mSearchTools->isHidden()){
        mSearchTools->setVisible(true);
        mSearchTools->show();
    }else
        mSearchTools->hide();

}

//_______________________________________________________
void  WidgetPdf::docSetFiteWith(int arg)
{
    QSettings s;
    s.beginGroup( filePath());
    s.setValue("FitSize",arg);
    mFitSize=arg;

    switch (arg) {
    case 0:
        mDocumentView->fitPageWith();
        break;
    case 1:
        mDocumentView->originalSize();
        break;
    case 2:
        mDocumentView->fitPageSize();
        break;
    default:
        break;
    }
}

//_______________________________________________________
void WidgetPdf::setLayoutMode(QAction* act)
{
    QSettings s;
    s.beginGroup( filePath());
    s.setValue("LayoutMode",act->data().toInt());

    switch (act->data().toInt()) {
    case 0:
        mDocumentView->setLayoutMode(mDocumentView->SinglePageMode);
        break;
    case 1:
        mDocumentView->setLayoutMode(mDocumentView->TwoPagesMode);
        break;
    default:
        break;
    }
}

//__________________________________________________________
void WidgetPdf::slotPageChanged(int page)
{

    //    mActFirst->setEnabled(page>1);
    //    mActPrev->setEnabled(page>1);
    //    mActNext->setEnabled(page<mDocumentView->numberOfPages());
    //    mActLast->setEnabled(page<mDocumentView->numberOfPages());

    emit   curPageChanged( page);

    if(ui->widgetContents->isHidden())return;

    if(ui->stackedWidget->currentIndex()==0)
    {


        int count=ui->treeViewPdf->model()->rowCount();

        if(count<1)return;
        if(ui->treeViewPdf->currentIndex().data(Qt::UserRole ).toInt()==page)
            return;
        for (int i = 0; i < count; ++i)
        {
            QModelIndex index=ui->treeViewPdf->model()->index(i,0);

            if(page== index.data(Qt::UserRole ).toInt()){
                ui->treeViewPdf->setCurrentIndex(index);
                return;
            }

            if(ui->treeViewPdf->model()->rowCount(index)>0)
                if (findIndex( index, page))
                    return;
        }

    }
    else if((ui->stackedWidget->currentIndex()==1))
    {

        int count=ui->listViewPdf->model()->rowCount();
        if(count<1)return;
        if(ui->listViewPdf->currentIndex().data(Qt::UserRole ).toInt()+ 1==page)
            return;


        for (int i = 0; i < count; ++i)
        {
            QModelIndex index=ui->listViewPdf->model()->index(i,0);

            if(page== index.data(Qt::UserRole ).toInt()+1){
                ui->listViewPdf->setCurrentIndex(index);
                return;
            }
        }
    }


}

bool WidgetPdf::findIndex(QModelIndex index,int page)
{
    int count=ui->treeViewPdf->model()->rowCount(index);

    for (int i = 0; i < count; ++i)
    {
        QModelIndex indexChild=index.child(i,0);

        if(page== indexChild.data(Qt::UserRole + 1).toInt()){
            ui->treeViewPdf->setCurrentIndex(indexChild);
            return true;
        }
        if(ui->treeViewPdf->model()->rowCount(indexChild)>0)
            if(findIndex( indexChild, page))
                return true;
    }
    return false;
}

//____________________________________________________________Bookmark
void WidgetPdf::addBookmark(const QString& str)
{

    if(!mDocumentView->isValid())return;

    QSizeF sizeF=  mDocumentView->curentPagePos();
    int page=mDocumentView->currentPage();
    qreal left=sizeF.rwidth();
    qreal top=sizeF.rheight();
    emit addNewBookmark(mDocumentView->filePath(), page, left, top,str);

}

//_______________________________________________________
void WidgetPdf::jumpToPage(int page,qreal left,qreal top)
{
    mDocumentView->jumpToPage(page, true, left, top);

}


//_______________________________________________________
void  WidgetPdf::docToggleContentVisible(bool arg)
{
    ui->widgetContents->setVisible(arg);
}
//_______________________________________________________
bool  WidgetPdf::isContentVisible()
{
    return   ui->widgetContents->isVisible();
}
//_______________________________________________________
void  WidgetPdf::docEmitFirstPage()
{
    mDocumentView->firstPage();
}
//_______________________________________________________
void  WidgetPdf::docEmitPreviousPage()
{
    mDocumentView->previousPage();
}
//_______________________________________________________
void  WidgetPdf::docEmitNextPage()
{
    mDocumentView->nextPage();
}
//_______________________________________________________
void  WidgetPdf::docEmitLastPage()
{
    mDocumentView->lastPage();
}
//_______________________________________________________
void  WidgetPdf::docEmitAddBookmark()
{
    addBookmark();
}
//_______________________________________________________
void  WidgetPdf::docRefresh()
{
    mDocumentView->refresh();
}
//_______________________________________________________
void WidgetPdf:: docEmitZoomIn()
{
    mDocumentView->zoomIn();
}
//_______________________________________________________
void  WidgetPdf::docEmitZoomOut()
{
    mDocumentView->zoomOut();
}
//_______________________________________________________
void  WidgetPdf::docEmitSetRubberBand(bool arg)
{
    mDocumentView->setRubberBand(arg);

}
//_______________________________________________________
bool  WidgetPdf::isRubberBand()
{
    return mDocumentView->isRubberBandMode();
}
//_______________________________________________________
void  WidgetPdf::docEmitSetInvertColors(bool arg)
{
    mInvertColors=arg;
    QSettings s;
    s.beginGroup( filePath());
    s.setValue("IvertColors",arg);

    mDocumentView->setInvertColors(arg);
}
//_______________________________________________________
void  WidgetPdf::docSetPage(int arg)
{
    mDocumentView->jumpToPage(arg);
}
//_______________________________________________________
void  WidgetPdf::docSetContinousMode(bool arg)
{
    mDocumentView->setContinousMode(arg);
}


//_______________________________________________________
void  WidgetPdf::docSetLayoutMode(int arg)
{
    QSettings s;
    s.beginGroup( filePath());
    s.setValue("LayoutMode",arg);

    switch (arg) {
    case 0:
        mDocumentView->setLayoutMode(mDocumentView->SinglePageMode);
        break;
    case 1:
        mDocumentView->setLayoutMode(mDocumentView->TwoPagesMode);
        break;
    default:
        break;
    }
}

//_______________________________________________________
void WidgetPdf::on_tabWidget_tabBarClicked(int index)
{
qDebug()<<"WidgetPdf::on_tabWidget_tabBarClicked(index):"<<index;
    if(ui->stackedWidget->currentIndex()==index) {
       showHideContent();
    }
    ui->stackedWidget->setCurrentIndex(index);
   // swithContents(index);

}
//_______________________________________________________
void WidgetPdf::on_tabWidget_currentChanged(int index)
{

    ui->stackedWidget->setCurrentIndex(index);

    ui->widgetContents->show();

    ui-> toolButtonShowHide->setArrowType(arrow(true));

  //  swithContents(index);

}

//_______________________________________________________
void WidgetPdf::showHideContent()
{
    if(ui->widgetContents->isHidden()){

        ui->widgetContents->show();
        ui-> toolButtonShowHide->setArrowType(arrow(true));
    }else{
        ui->widgetContents->hide();
        ui-> toolButtonShowHide->setArrowType(arrow(false));
    }
}

//_______________________________________________________
Qt::ArrowType WidgetPdf::arrow(bool opened)
{
    if(isRightToLeft()){

        if(opened)
            return Qt::RightArrow;
        else
            return Qt::LeftArrow;

    }else{

        if(opened)
            return Qt::LeftArrow;
        else
            return Qt::RightArrow;

    }
}

QStandardItemModel* WidgetPdf::propertiesModel()
{
    return mDocumentView->propertiesModel();
}

QStandardItemModel* WidgetPdf::fontsModel()
{
   return  mDocumentView->fontsModel();
}

void WidgetPdf::saveLogoPixmap(const QString &file)
{

    QPixmap pix=QPixmap::fromImage(mDocumentView->imagePage(0,QSizeF(256.0,256.0)));
    pix.save(file);

}
