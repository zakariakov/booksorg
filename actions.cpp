#include "actions.h"
#include "eicon.h"
#include <QSettings>

Actions::Actions(QLocale lc, QObject *parent) :
    QObject(parent),mlocale(lc)
{

  setupBooksActions();
  setupPdfsActions();
}

//________________________________________BOOKS
void Actions::setupBooksActions()
{
    QSettings setting;
    mActEditBook=new QAction(tr("Edit..."),this);
    mActEditBook->setStatusTip(tr("Edit select book or group"));
    mActEditBook->setIcon(EIcon::icon(EDIT));
    mActEditBook->setShortcut(QKeySequence("Ctrl+E"));
    mActEditBook->setToolTip(tr("Edit select book or group")+"\n"+mActEditBook->shortcut().toString());

    mActDelete=new QAction(tr("Delete"),this);
    mActDelete->setStatusTip(tr("Delete selected books or group"));
    mActDelete->setIcon(EIcon::icon(REMOVE));
    mActDelete->setShortcut(QKeySequence::Delete);
    mActDelete->setToolTip(tr("Delete selected books or group")+"\n"+mActDelete->shortcut().toString());

    mActRootPath=new QAction(tr("Home"),this);
    mActRootPath->setStatusTip(tr("Home"));
    mActRootPath->setIcon(EIcon::icon(GO_HOME));
    mActRootPath->setShortcut(QKeySequence("Home"));
    mActRootPath->setToolTip(tr("Go home")+"\n"+mActRootPath->shortcut().toString());

    //View Mode Actions
    int vMode=setting.value("ViewMode",0).toInt();
    actIconView = new QAction(tr("Icon view"),this);
    actIconView->setStatusTip(tr("Toggle icon view"));
    actIconView->setCheckable(true);
    actIconView->setChecked(!vMode);
    actIconView->setData(IconView);
    actIconView->setIcon(EIcon::icon(VIEW_ICONS));
    actIconView->setShortcut(QKeySequence("Ctrl+1"));
    actIconView->setToolTip(tr("Toggle icon view")+"\n"+actIconView->shortcut().toString());

    actDetailView = new QAction(tr("Detail view"),this);
    actDetailView->setStatusTip(tr("Toggle detailed list"));
    actDetailView->setCheckable(true);
    actDetailView->setChecked(vMode);
    actDetailView->setData(DetailView);
    actDetailView->setIcon(EIcon::icon(VIEW_DETAILS));
    actDetailView->setShortcut(QKeySequence("Ctrl+2"));
    actDetailView->setToolTip(tr("Toggle detailed list")+"\n"+actDetailView->shortcut().toString());

    alignViewMode = new QActionGroup(this);
    alignViewMode->addAction(actIconView);
    alignViewMode->addAction(actDetailView);
    emit viewModeChanged(vMode);

     actViewBook=new QAction(this);
  menuViewBook=new QMenu;
    menuViewBook->addAction(actIconView);
    menuViewBook->addAction(actDetailView);

    actViewBook->setMenu(menuViewBook);
actViewBook->setActionGroup(alignViewMode);
    switch (vMode) {
    case 0:
        toggleView(actIconView);
        break;
    case 1:
        toggleView(actDetailView);
        break;

    default:
        break;
    }
    // Category Actions
    int cat=setting.value("Category",0).toInt();
    mActGroopPath=new QAction(tr("Groops"),this);
    mActGroopPath->setIcon(EIcon::icon(GROOPS));
    mActGroopPath->setCheckable(true);
    mActGroopPath->setChecked(cat==0);
    mActGroopPath->setData(CAT_GROUP);
    mActGroopPath->setShortcut(QKeySequence("Ctrl+G"));
    mActGroopPath->setToolTip(tr("Groops")+"\n"+mActGroopPath->shortcut().toString());

    mActAutorPath=new QAction(tr("Authors"),this);
    mActAutorPath->setIcon(EIcon::icon(AUTORS));
    mActAutorPath->setCheckable(true);
    mActAutorPath->setChecked(cat==1);
    mActAutorPath->setData(CAT_AUTHOR);
    mActAutorPath->setShortcut(QKeySequence("Ctrl+U"));
    mActAutorPath->setToolTip(tr("Autors")+"\n"+mActAutorPath->shortcut().toString());

    mActFavoPath=new QAction(tr("Rating"),this);
    mActFavoPath->setIcon(EIcon::icon(FAVO));
    mActFavoPath->setCheckable(true);
    mActFavoPath->setChecked(cat==2);
    mActFavoPath->setData(CAT_FAVORIT);
    mActFavoPath->setShortcut(QKeySequence("Ctrl+R"));
    mActFavoPath->setToolTip(tr("Rating")+"\n"+mActFavoPath->shortcut().toString());

    alignRootPath = new QActionGroup(this);
    alignRootPath->addAction(mActGroopPath);
    alignRootPath->addAction(mActAutorPath);
    alignRootPath->addAction(mActFavoPath);
    emit categoryChanged(cat);

    actPath=new QAction(this);
menuPath=new QMenu;
    menuPath->addAction(mActGroopPath);
    menuPath->addAction(mActAutorPath);
    menuPath->addAction(mActFavoPath);
    actPath->setMenu(menuPath);

actPath->setActionGroup(alignRootPath);
switch (cat) {
case 0:
    toggleRootPath(mActGroopPath);
    break;
case 1:
    toggleRootPath(mActAutorPath);
    break;
case 2:
    toggleRootPath(mActFavoPath);
    break;
default:
    break;
}
    //Cnnections
    connect(alignViewMode, SIGNAL(triggered(QAction*)),this, SLOT(toggleView(QAction*)));
    connect(mActRootPath,SIGNAL(triggered()),this,SIGNAL(emitRootPath()));
    connect(mActEditBook,SIGNAL(triggered()),this,SIGNAL(emitEditBookOrGroup()));
    connect(mActDelete,SIGNAL(triggered()),this,SIGNAL(emitDeletSelectedIndexes()));
    connect(alignRootPath, SIGNAL(triggered(QAction*)),this, SLOT(toggleRootPath(QAction*)));

    sepBook=new QAction(this);
    sepBook->setSeparator(true);
    sepBook2=new QAction(this);
    sepBook2->setSeparator(true);
    sepBook3=new QAction(this);
   sepBook3->setSeparator(true);


  // mListViewBooks<<actIconView<<actDetailView;
    //mListEditBooks<<mActEditBook<<mActDelete;


}
QList<QAction *>Actions::listEditBooks()
{
   return QList<QAction *>() <<mActEditBook<<mActDelete;

}

QList<QAction *>Actions::listViewBooks()
{
    return QList<QAction *>()<<actIconView<<actDetailView;

}

QList<QAction *>Actions::listCatBooks()
{
    return QList<QAction *>()
            <<mActRootPath
           <<sepBook
          <<mActGroopPath
         <<mActAutorPath
        <<mActFavoPath;

}

QList<QAction *> Actions::listStandardToolBarBooks()
{

    return QList<QAction *>()
            <<listEditBooks()<<sepBook2<<actPath<<sepBook3<<listViewBooks();
}

QList<QAction *> Actions::listToolBarBooks()
{
     return QList<QAction *>()
             <<actPath<<actViewBook;
}

QList<QAction *>Actions::listAllBooksActions()
{
    return QList<QAction *>()
            <<mActDelete <<mActEditBook<<mActRootPath
           <<mActGroopPath<<mActAutorPath  <<mActFavoPath
          <<actIconView<<actDetailView <<actViewBook
         <<actPath<<sepBook <<sepBook2<<sepBook3;


}

//________________________________________BOOKS
void Actions::toggleView(QAction* action)
{

    int mode=action->data().toInt();
    emit viewModeChanged(mode);
    QSettings setting;
    setting.setValue("ViewMode",mode);

    switch (mode) {
    case 0:
         actIconView->setChecked(true);
        actViewBook->setIcon(actDetailView->icon());
        actViewBook->setData(actDetailView->data());
        actViewBook->setText(actDetailView->text());
        break;
    case 1:
        actDetailView->setChecked(true);
        actViewBook->setIcon(actIconView->icon());
        actViewBook->setData(actIconView->data());
        actViewBook->setText(actIconView->text());
        break;

    default:
        break;
    }


}

//_________________________________________BOOKS
void Actions::toggleRootPath(QAction* action)
{
    int cat=action->data().toInt();
   emit categoryChanged(cat);
    QSettings setting;
    setting.setValue("Category",cat);
    actPath->setIcon(action->icon());
    actPath->setData(action->data());
    actPath->setText(action->text());

}

//_________________________________________PDFS
void Actions:: setupPdfsActions()
{

    QSettings setting;

    // show/hide contents-------
//    mActShowHide=new QAction(EIcon::icon(VIEW_CLOSE) ,tr("show/hide contents"),this);
//    mActShowHide->setCheckable(true);
//    mActShowHide->setShortcut(QKeySequence("Ctrl+L"));
//    mActShowHide->setToolTip(mActShowHide->toolTip()+"\n"+mActShowHide->shortcut().toString());
//    mActShowHide->setStatusTip(mActShowHide->toolTip().replace("\n","  "));
   // connect(mActShowHide,SIGNAL(toggled(bool)),this,SIGNAL(pdfToggleContentVisible(bool)));


    // first page---------
    mActFirst=new   QAction(EIcon::icon(GO_FIRST) ,tr("&First page"),this);
    mActFirst->setShortcut(QKeySequence::MoveToStartOfDocument);
    mActFirst->setToolTip(mActFirst->toolTip()+"\n"+mActFirst->shortcut().toString());
    mActFirst->setStatusTip(mActFirst->toolTip().replace("\n","  "));
    //mActFirst->setEnabled(false);
    connect(mActFirst,SIGNAL(triggered()),this,SIGNAL(pdfEmitFirstPage()));

    // previous page-----
    mActPrev=new   QAction(EIcon::icon(GO_PREVIOUS)  ,tr("&Previous page"),this);
    mActPrev->setShortcut(QKeySequence::MoveToPreviousPage);
    mActPrev->setToolTip(mActPrev->toolTip()+"\n"+mActPrev->shortcut().toString());
    mActPrev->setStatusTip(mActPrev->toolTip().replace("\n","  "));
   // mActPrev->setEnabled(false);
    connect(mActPrev,SIGNAL(triggered()),this,SIGNAL(pdfEmitPreviousPage()));


    // Spin go page-----------
    mActSpin=new QWidgetAction(this);
    mSpinBox=new QSpinBox;

    mSpinBox->setRange(0,0);
    mActSpin->setDefaultWidget(mSpinBox);

    mSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(mSpinBox,SIGNAL(editingFinished()),this,SLOT(pdfGoPage()));
    connect(this,SIGNAL(currentPageChanged(int)),mSpinBox,SLOT(setValue(int)));

    // next page---------
    mActNext=new   QAction(EIcon::icon(GO_NEXT) ,tr("&Next page"),this);
    mActNext->setShortcut(QKeySequence::MoveToNextPage);
    mActNext->setToolTip(mActNext->toolTip()+"\n"+mActNext->shortcut().toString());
    mActNext->setStatusTip(mActNext->toolTip().replace("\n","  "));
   // mActNext->setEnabled(false);
    connect(mActNext,SIGNAL(triggered()),this,SIGNAL(pdfEmitNextPage()));


    // last page---------
    mActLast=new   QAction(EIcon::icon(GO_LAST),tr("&Last page"),this );
    mActLast->setShortcut(QKeySequence::MoveToEndOfDocument);
    mActLast->setToolTip(mActLast->toolTip()+"\n"+mActLast->shortcut().toString());
    mActLast->setStatusTip(mActLast->toolTip().replace("\n","  "));
    //mActLast->setEnabled(false);
    connect(mActLast,SIGNAL(triggered()),this,SIGNAL(pdfEmitLastPage()));


    // Bookmark---------
    mActAddBookmark= new QAction(EIcon::icon(BOOKMARK_ADD),tr("&Add Bookmark"),this);
    mActAddBookmark->setShortcut(QKeySequence("Ctrl+B"));
    connect(mActAddBookmark,SIGNAL(triggered()),this,SIGNAL(pdfEmitAddBookmark()));


    // Refresh---------
    mActRefresh = new QAction(tr("&Refresh"),this);
    mActRefresh->setShortcut(QKeySequence::Refresh);
    mActRefresh->setIcon(EIcon::icon(RELOAD));
    connect(mActRefresh,SIGNAL(triggered()),this,SIGNAL(pdfRefresh()));

    mActEditBookInfo= new QAction(tr("&Edit Book Info"),this);
    //mActEditBook->setShortcut(QKeySequence::Refresh);
    mActEditBookInfo->setIcon(EIcon::icon(INFO_HELP));
    connect(mActEditBookInfo,SIGNAL(triggered()),this,SIGNAL(pdfEditBook()));

    // find------------------
    //    a= new QAction(EIcon::icon(FIND),tr("&Find"),this);
    //    connect(a,SIGNAL(triggered()),this,SLOT(showHideSearch()));
    //    a->setShortcut(QKeySequence::Find);
    //    a->setToolTip(a->toolTip()+"\n"+a->shortcut().toString());
    //    a->setStatusTip(a->toolTip().replace("\n","  "));
    //    mListEdit.append(a);

    // zoom in-----------
    mActZoomIn= new QAction(EIcon::icon(ZOOM_IN) ,tr("Zoom &in"),this);
    mActZoomIn->setShortcut(QKeySequence::ZoomIn);
    mActZoomIn->setToolTip(mActZoomIn->toolTip()+"\n"+mActZoomIn->shortcut().toString());
    mActZoomIn->setStatusTip(mActZoomIn->toolTip().replace("\n","  "));

    connect(mActZoomIn,SIGNAL(triggered()),this,SLOT(disableFite()));
    connect(mActZoomIn,SIGNAL(triggered()),this,SIGNAL(pdfEmitZoomIn()));

    // zoom out----------
    mActZoomOut= new QAction(EIcon::icon(ZOOM_OUT)   ,tr("Zoom &out"),this);
      mActZoomOut->setShortcut(QKeySequence::ZoomOut);
    mActZoomOut->setToolTip(mActZoomOut->toolTip()+"\n"+mActZoomOut->shortcut().toString());
    mActZoomOut->setStatusTip(mActZoomOut->toolTip().replace("\n","  "));
    connect(mActZoomOut,SIGNAL(triggered()),this,SLOT(disableFite()));
    connect(mActZoomOut,SIGNAL(triggered()),this,SIGNAL(pdfEmitZoomOut()));

    //mToolBarDoc->addSeparator();

    // Aligne fite size-------
    mAlignActionsFite = new QActionGroup(this);
    int mFitSize=  setting.value("FitSize",0).toInt();

    // original size-----
    mActOriginalSize=  new QAction(EIcon::icon(ZOOM_ORIG),tr("Original &size"),this);
    mActOriginalSize->setStatusTip(mActOriginalSize->toolTip());
    mActOriginalSize->setCheckable(true);
    mActOriginalSize->setData(1);
    mActOriginalSize->setStatusTip(mActOriginalSize->toolTip());

    mAlignActionsFite->addAction(mActOriginalSize);
    mActOriginalSize->setChecked(mFitSize==1);

    // fit to page width------
    mActZoomFit=  new QAction(EIcon::icon(ZOOM_FIT),tr("Fit width"),this);
    mActZoomFit->setCheckable(true);
    mActZoomFit->setData(0);
    mActZoomFit->setStatusTip(mActZoomFit->toolTip());

    mAlignActionsFite->addAction(mActZoomFit);
    mActZoomFit->setChecked(mFitSize==0);

    // fit to page size-----
    mActZoomBest= new QAction(EIcon::icon(ZOOM_BEST),tr("Fit to page size"),this);
    mActZoomBest->setCheckable(true);
    mActZoomBest->setData(2);
    mActZoomBest->setStatusTip(mActZoomBest->toolTip());

    mAlignActionsFite->addAction(mActZoomBest);
    mActZoomBest->setChecked(mFitSize==2);

    connect(mAlignActionsFite,SIGNAL(triggered(QAction*)),this ,SLOT(setFitSizeAct(QAction*)));
    emit  pdfSetFiteWith(mFitSize );


    mActSizeFite=new QAction(this);
    menuSizeFite=new QMenu;
menuSizeFite->addAction(mActZoomFit);
menuSizeFite->addAction(mActOriginalSize);
menuSizeFite->addAction(mActZoomBest);
mActSizeFite->setMenu(menuSizeFite);
mActSizeFite->setActionGroup(mAlignActionsFite);
    switch (mFitSize) {
    case 0:
       setFitSizeAct(mActZoomFit);
        break;
    case 1:
       setFitSizeAct(mActOriginalSize);
        break;
    case 2:
        setFitSizeAct(mActZoomBest);
        break;
    default:
        break;
    }

    // Separator ---------



    bool continueMode=setting.value("ContinousMode",true).toBool();

    // continousmode-----
    mActContinous= new QAction(EIcon::icon(VIEW_LIST),tr("Continous"),this);
    connect(mActContinous,SIGNAL(toggled(bool)),this,SIGNAL(pdfSetContinousMode(bool)));
    mActContinous->setCheckable(true);
    mActContinous->setChecked(continueMode);
    mActContinous->setStatusTip(mActContinous->toolTip());
    emit pdfSetContinousMode(continueMode);


    // rubermode-----
   mActRubberBand=  new QAction(QIcon(":/icons/cross.png"),tr("Selection"),this);
    connect(mActRubberBand,SIGNAL(toggled(bool)),this,SIGNAL(pdfEmitSetRubberBand(bool)));
    connect(mActRubberBand,SIGNAL(toggled(bool)),this,SLOT(setRubberBandAct(bool)));

 //   connect(mActRubberBand,SIGNAL(toggled(bool)),this,SLOT(toggleRubberBand(bool)));
    mActRubberBand->setShortcut(QKeySequence("Ctrl+S"));
    mActRubberBand->setCheckable(true);
    mActRubberBand->setChecked(false);
    mActRubberBand->setStatusTip(mActRubberBand->toolTip());

    bool invert=setting.value("IvertColors",false).toBool();

    // Invert colors-------
    mActInvertcolors= new QAction(tr("Invert colors"),this);
    mActInvertcolors->setStatusTip(tr("Invert colors... "));
    mActInvertcolors->setShortcut(QKeySequence("Ctrl+Shift+c"));
    mActInvertcolors->setToolTip(tr("Invert colors")+"\n"+mActInvertcolors->shortcut().toString());
    mActInvertcolors->setCheckable(true);
    mActInvertcolors->setChecked(invert);
        connect(mActInvertcolors,SIGNAL(toggled(bool)),this,SIGNAL(pdfEmitSetInvertColors(bool)));
    //  mDocumentView->setInvertColors(invert);

    // alignLayoutMode------
    alignLayoutMode = new QActionGroup(this);
    int lmode=setting.value("LayoutMode",0).toInt();



    // single Page-------------
    mActSinglePage= new QAction(tr("Single Page"),this);
    mActSinglePage->setCheckable(true);
    mActSinglePage->setData(0);
    mActSinglePage->setChecked(lmode==0);
    alignLayoutMode->addAction(mActSinglePage);


    // Two Pages--------------
    mActTwoPages= new QAction(tr("Two Pages"),this);
    mActTwoPages->setCheckable(true);
    mActTwoPages->setData(1);
    mActTwoPages->setChecked(lmode==1);
    alignLayoutMode->addAction(mActTwoPages);


    connect(alignLayoutMode,SIGNAL(triggered(QAction*)),this
            ,SLOT(setLayoutModeAct(QAction*)));

    switch (lmode) {
    //    case 0: mDocumentView->setLayoutMode(mDocumentView->SinglePageMode);break;
    //    case 1:mDocumentView->setLayoutMode(mDocumentView->TwoPagesMode); break;
    default: break;
    }
    /*
*/
   sepPdf=new QAction(this);
    sepPdf->setSeparator(true);
   sepPdf2=new QAction(this);
   sepPdf2->setSeparator(true);
   sepPdf3=new QAction(this);
    sepPdf3->setSeparator(true);
    sepPdf4=new QAction(this);
   sepPdf4->setSeparator(true);

  //  mListViewPdf.append(mActShowHide);
//    mListViewPdf.append(sepPdf);
//    mListViewPdf.append(mActZoomIn);
//    mListViewPdf.append(mActZoomOut);
//    mListViewPdf.append(mActOriginalSize);
//    mListViewPdf.append(mActZoomFit);
//    mListViewPdf.append(mActZoomBest);
//    mListViewPdf.append(sepPdf2);
//    mListViewPdf.append(mActContinous);
//    mListViewPdf.append(mActInvertcolors);
//    mListViewPdf.append(sepPdf3);
//    mListViewPdf.append(mActSinglePage);
//    mListViewPdf.append(mActTwoPages);
//    mListViewPdf.append(sepPdf4);
//    mListViewPdf.append(mActRubberBand);

    //-----------------------------------------


//    mListEditPdf.append(mActFirst);
//    mListEditPdf.append(mActPrev);
//    mListEditPdf.append(mActNext);
//    mListEditPdf.append(mActLast);
//    mListEditPdf.append(sepPdf4);
//    mListEditPdf.append(mActRefresh);
//    mListEditPdf.append(mActAddBookmark);
//    mListEditPdf.append(mActEditBookInfo);


}

QList<QAction *>Actions::listViewPdf()
{
    return  QList<QAction *>()
            <<(sepPdf)<<(mActZoomIn) <<(mActZoomOut)
           <<(mActOriginalSize)<<(mActZoomFit)<<(mActZoomBest)
          <<(sepPdf2)<<(mActContinous)<<(mActInvertcolors)
         <<(sepPdf3)<<(mActSinglePage)<<(mActTwoPages)
        <<(sepPdf4)<<(mActRubberBand);

}
 QList<QAction *>Actions::listEditPdf()
 {
     return  QList<QAction *>()
             <<mActFirst<<(mActPrev) <<(mActNext)
            <<(mActLast)<<(sepPdf4) <<(mActRefresh)
           <<(mActEditBookInfo)<<(mActAddBookmark) ;


 }

QList<QAction *>Actions::listStandardToolBarPdf()
{
    return  QList<QAction *>()
            <<mActFirst<<mActPrev<<mActSpin
           <<mActNext <<mActLast<<sepPdf2
          <<mActSizeFite<<sepPdf3<<mActRubberBand
         <<sepPdf4<<mActContinous<<mActEditBookInfo;


}
 QList<QAction *>Actions::listToolBarPdf()
 {
     return  QList<QAction *>()<<mActSpin<<mActSizeFite<<mActRubberBand;
 }
 QList<QAction *>Actions::listAllPdfActions()
 {
     return  QList<QAction *>()
             <<mActFirst<<mActPrev<<mActNext<<mActLast<<mActSpin
            <<mActAddBookmark<<mActRefresh<<mActZoomIn<<mActZoomOut
           <<mActOriginalSize<<mActZoomFit<<mActZoomBest<<mActSizeFite
          <<mActContinous<<mActRubberBand<<mActInvertcolors
         <<mActSinglePage<<mActTwoPages<<mActEditBookInfo<<sepPdf
        <<sepPdf2<<sepPdf3<<sepPdf4;

 }
//_______________________________________________________
void Actions::pdfGoPage()
{
    emit pdfSetPage(mSpinBox->value());
}

//_______________________________________________________
void Actions::disableFite()
{

    foreach (QAction *act, mAlignActionsFite->actions())
        act->setChecked(false);

}

//__________________________________________________________
void Actions::setFitSizeAct(QAction* act)
{

mActSizeFite->setIcon(act->icon());
mActSizeFite->setText(act->text());
mActSizeFite->setData(act->data().toInt());

   emit  pdfSetFiteWith(act->data().toInt() );

}
void Actions::setFitSizeAct( int fit)
{
    switch (fit) {
    case 0:
        mActZoomFit->setChecked(true);
        break;
    case 1:
        mActOriginalSize->setChecked(true);
        break;
    case 2:
        mActZoomBest->setChecked(true);
        break;
    default:
        break;
    }
}

//__________________________________________________________
void Actions::setLayoutModeAct(QAction *act)
{

emit pdfSetLayoutMode(act->data().toInt());

}

void Actions:: setLayoutModeAct(int layout)
 {
    switch (layout) {
    case 0:
        mActSinglePage->setChecked(true);
        break;
    case 1:
        mActTwoPages->setChecked(true);
        break;
    default:
        break;
    }
 }

//_______________________________________________________
void Actions:: setSpinPages(int numPages,int page)
{
    QString num=mlocale.toString(numPages);

    //   QString num=QString::number(mDocumentView->numberOfPages());
    mSpinBox-> setSuffix(QString(" %1 %2 ").arg(tr("of")).arg(num));
    mSpinBox->setRange(1,numPages);
    mSpinBox->setValue(page);
}

//_______________________________________________________
void  Actions::setRubberBandAct(bool arg)
{
  mActRubberBand->setChecked(arg);
    if(arg){
        mActRubberBand->setText(tr("Hand"));
        mActRubberBand->setIcon(QIcon(":/icons/closedhand.png"));
    } else{
        mActRubberBand->setText(tr("Selection"));
        mActRubberBand->setIcon(QIcon(":/icons/cross.png"));
    }
}

//_______________________________________________________
//void  Actions::setPdfContentVisibleAct(bool arg)
//{

//      mActShowHide->setChecked(arg);

//}

void  Actions::setContinousModeAct(bool arg)
{
    mActContinous->setChecked( arg);
}

void  Actions::setInvertcolorsAct(bool arg)
{
    mActInvertcolors->setChecked( arg);
}
