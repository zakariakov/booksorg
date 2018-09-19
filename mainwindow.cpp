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
#include "mainwindow.h"
#include "eicon.h"
#include "ui_mainwindow.h"
#include "addfolderwizard.h"
#include "editbookdialog.h"
#include "addbookwizard.h"
#include "bookinfo.h"
#include "configuredialog.h"
#include "path.h"
#include <QDebug>
#include <QInputDialog>
#include <QDirIterator>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QFontMetrics>
#include <QErrorMessage>
#include <QImageReader>
#include <QTextCodec>
#include <QFileInfo>
//#define thumbnails QDir::homePath()+"/."+QApplication::applicationName()+"/thumbnails"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),mDebug(true)
{

  qDebug()<<QString("*************************************************************************\n"
               "****************BOOKSORG V: %1  By About Zakaria*************************\n"
               "*booksorg is a easy organizer for your PDF books using  poppler library.**\n"
               "******See https://elkirtasse.sourceforge.net for more information.********\n"
               "********************booksorg --debug for debug mode***********************\n"
               "*************************************************************************")
            .arg(QApplication::applicationVersion());

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QIcon::setThemeName(EIcon::themeName());
    ui->setupUi(this);
    //---------------------------------------------
QSettings settings;
    settings.beginGroup("Main");
    restoreGeometry(settings.value("Geometry").toByteArray());
    settings.endGroup();
    //qDebug()<<"loadGeometry";
    //---------------------------------------------



   Path::setWorkingDir(settings.value("BooksPath",
                             QDir::homePath()+"/."+QApplication::applicationName()).toString());

   Path::setCopyBooks(settings.value("CopyBooks",false).toBool());


    ui->labelImageInfo->setPixmap(EIcon::icon(INFO_HELP).pixmap(64,64)/*QPixmap(style()->standardPixmap(QStyle::SP_MessageBoxInformation))*/);
    setAcceptDrops(true);
     //mPath=new Path;

    mModel=new MyModel;



    mModel->openDataBase();

    mSearchTools =new SearchTools(false);

    mActions=new Actions(this->locale());

    mRatingWidget =new RatingWidget;

    //  mPopupForm=new PopupForm(this);

    ui->hLayoutRattings->addWidget(mRatingWidget);
    mRatingWidget->setVisible(false);
    connect(mRatingWidget,SIGNAL(ratingChanged(int,QString)),mModel,SLOT(saveRating(int,QString)));

    connect(mActions,SIGNAL(categoryChanged(int)),mModel,SLOT(setCategory(int)));
    connect(mActions,SIGNAL(viewModeChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(mActions,SIGNAL(emitDeletSelectedIndexes()),this,SLOT(deletSelectedIndexes()));
    connect(mActions,SIGNAL(emitEditBookOrGroup()),this,SLOT(editBookOrGroup()));


    connect(mActions,SIGNAL(emitRootPath()),this,SLOT(goRootPath()));

    connect(this,SIGNAL(curPageChanged(int)),mActions,SIGNAL(currentPageChanged(int)));
    // connect(mActions,SIGNAL(pdfToggleContentVisible(bool)),this,SLOT(wPdfToggleContentVisible(bool)));
    connect(mActions,SIGNAL(pdfEmitAddBookmark()),this,SLOT(wPdfEmitAddBookmark()));
    connect(mActions,SIGNAL(pdfEmitFirstPage()),this,SLOT(wPdfEmitFirstPage()));
    connect(mActions,SIGNAL(pdfEmitLastPage()),this,SLOT(wPdfEmitLastPage()));
    connect(mActions,SIGNAL(pdfEmitNextPage()),this,SLOT(wPdfEmitNextPage()));
    connect(mActions,SIGNAL(pdfEmitPreviousPage()),this,SLOT(wPdfEmitPreviousPage()));
    connect(mActions,SIGNAL(pdfEmitSetInvertColors(bool)),this,SLOT(wPdfEmitSetInvertColors(bool)));
    connect(mActions,SIGNAL(pdfEmitSetRubberBand(bool)),this,SLOT(wPdfEmitSetRubberBand(bool)));
    connect(mActions,SIGNAL(pdfEmitZoomIn()),this,SLOT(wPdfEmitZoomIn()));
    connect(mActions,SIGNAL(pdfEmitZoomOut()),this,SLOT(wPdfEmitZoomOut()));
    connect(mActions,SIGNAL(pdfSetContinousMode(bool)),this,SLOT(wPdfSetContinousMode(bool)));
    connect(mActions,SIGNAL(pdfSetFiteWith(int)),this,SLOT(wPdfSetFiteWith(int)));
    connect(mActions,SIGNAL(pdfSetLayoutMode(int)),this,SLOT(wPdfSetLayoutMode(int)));
    connect(mActions,SIGNAL(pdfSetPage(int)),this,SLOT(wPdfSetPage(int)));
    connect(mActions,SIGNAL(pdfRefresh()),this,SLOT(wPdfRefresh()));
    connect(mActions,SIGNAL(pdfEditBook()),this,SLOT(editBookInfo()));

    setupTools();

    ui->dockWidgetBookmark->setVisible(false);

    QFontMetrics fm(ui->listView->font());
    ui->listView->setGridSize(QSize(135,135+(fm.height()*2)));

    ui->listView->setModel(mModel);
    ui->treeView->setModel(mModel);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenu(QPoint)));
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenu(QPoint)));

    connect( ui->listView,SIGNAL(activated(QModelIndex)),this,SLOT(slotItemActivated(QModelIndex)));
    connect( ui->treeView,SIGNAL(activated(QModelIndex)),this,SLOT(slotItemActivated(QModelIndex)));
    connect( ui->listView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
             ,this,SLOT(slotcurrentRowChanged(QModelIndex,QModelIndex)));
    connect( ui->treeView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
             ,this,SLOT(slotcurrentRowChanged(QModelIndex,QModelIndex)));

    connect(mModel,SIGNAL(showMessage(QString)),ui->statusBar,SLOT(showMessage(QString)));
    connect(mModel,SIGNAL(pathChanged(QString)),ui->statusBar,SLOT(showMessage(QString)));
    connect(mModel,SIGNAL(pathChanged(QString)),this,SLOT(pathChanged(QString)));
    connect(mModel,SIGNAL(updateItem(QModelIndex)),this,SLOT(updateItem(QModelIndex)));

    connect(mSearchTools,SIGNAL(searchPressed(QString)),this,SLOT(goSearch(QString)));

    //-----------------------------------------------------
    mWidgetBookmark=new WidgetBookmark;
    ui->vLayoutBookmark->addWidget(mWidgetBookmark);
    connect(mWidgetBookmark,SIGNAL(getBookmark()),this,SLOT(slotAddBookmark()));
    connect(mWidgetBookmark,SIGNAL(openDocument(QString,int,qreal,qreal))
            ,this,SLOT(bookmarkOpenDocument(QString,int,qreal,qreal)));
    //---------------------------------------------


    setCurentTab(0);
 //if(isRightToLeft())
 //      ui->dockWidgetBookmark->setAllowedAreas(Qt::RightDockWidgetArea);
 if(QApplication::layoutDirection()==Qt::RightToLeft)
     addDockWidget(Qt::RightDockWidgetArea,ui->dockWidgetBookmark);


    QStringList args = QApplication::arguments();

    if(args.count()>1)
    {
        QUrl url = args.at(1);
        //  QMessageBox::information(this,"",url.toLocalFile());

        qDebug()<<"MainWindow::MainWindow() url:"<<url;
        QByteArray encodedString =QByteArray::fromPercentEncoding(url.toString().toUtf8());
        QString string = codec->toUnicode(encodedString);

        if(string=="--debug"){
            mDebug=true;
        }else{

            QFileInfo fi(string);

            qDebug()<<"fi"<<fi.absoluteFilePath();

            if (fi.exists() && fi.suffix().toLower()=="pdf" )
            {
                qDebug() << fi.filePath()  ;
                slotOpenFileName(fi.filePath());
            }
        }


    }


}

MainWindow::~MainWindow()
{
   // if(mDebug)
    qDebug()<<"MainWindow::~MainWindow() saveGeometry";





    delete ui;

}

void MainWindow::closeEvent(QCloseEvent *event)
 {
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        WidgetPdf   *w= qobject_cast<WidgetPdf *>( ui->tabWidget->widget(i));
        if(w){
            QString path=w->filePath();
            int p=  w->curPage();
            qDebug()<<"MainWindow::closeEvent() savePage:"<<path << p;
            mModel->savePage(path,p);

        }
    }
//    if(widgetPdf()){
//       QString path=widgetPdf()->filePath();
//       int p=  widgetPdf()->curPage();
//         qDebug()<<"MainWindow::~MainWindow() "<<path << p;
//            mModel->savePage(path,p);
//    }

    QSettings settings;
    settings.beginGroup("Main");
    settings.setValue("Geometry", saveGeometry());
    settings.endGroup();


     event->accept();

 }

void MainWindow::pathChanged(QString)
{
if(mDebug)
    qDebug()<<"MainWindow::pathChanged";

    if(focusWidget()==ui->treeView)
    {
        ui->treeView->resizeColumnToContents(0);
        ui->treeView->resizeColumnToContents(4);
    }

}

void MainWindow::setupTools()
{

    if(mDebug)
        qDebug()<<"MainWindow::setupTools";

    QSettings setting;

    ui->hLayoutBooks->addWidget(mSearchTools);
    mSearchTools->setVisible(false);


    /*!
            انشاء الاوامر اللازمة
            ثم اظافة الاوامر الى القائمة واللوحة
     */

    mActBooksTab=new QAction(tr("Books"),this);
    mActScanFolder=new QAction(tr("Scan folder"),this);
    mActOpen=new QAction(tr("Open"),this);
    mActOpenWith=new QAction(tr("Open With..."),this);
    mActMoveToAuthor=new QAction(tr("Change Author"),this);
    mActMoveToGroup=new QAction(tr("Change Group"),this);
    mActAddBook=new QAction(tr("Add book"),this);
     mActAddCurrent=new QAction(tr("Add current book"),this);
     mActAddCurrent->setEnabled(false);
    mActShowFullScreen = new QAction(tr("&Fullscreen"), this);
    mActAbout=new QAction(tr("About"),this);
    mActStandardMode=new QAction(tr("Standard Tool Bar"),this);
    mActFind= new QAction(EIcon::icon(FIND),tr("&Find"),this);
    mActConfig=new QAction(tr("Options"),this);
    QAction *actBookmark=ui->dockWidgetBookmark->toggleViewAction();
mActcleanBooks= new QAction(EIcon::icon(FIND),tr("&Clean"),this);
    //Shortcuts
    mActBooksTab->setShortcut(QKeySequence(QKeySequence::AddTab));
    mActScanFolder->setShortcut(QKeySequence("Ctrl+W"));
     mActConfig->setShortcut(QKeySequence("Ctrl+Shift+O"));
    mActAddBook->setShortcut(QKeySequence::Open);
    mActShowFullScreen->setShortcut(QKeySequence("F11"));
    mActFind->setShortcut(QKeySequence::Find);

    //StatusTip
    mActBooksTab->setStatusTip(tr("show books tab"));
    mActScanFolder->setStatusTip(tr("scan folder for existing books"));
    mActOpen->setStatusTip(tr("Open... "));
    mActOpenWith->setStatusTip(tr("Open book with default programme"));
    mActMoveToAuthor->setStatusTip(tr("Change Author for selected books"));
    mActMoveToGroup->setStatusTip(tr("Change Group for selected books"));
    mActAddBook->setStatusTip(tr("Add new book"));
      mActAddCurrent->setStatusTip(tr("Add current book"));
    mActAbout->setStatusTip(tr("About... "));
    //mActFind->setStatusTip();
     // mActConfig->setStatusTip(tr("Add new book"));

    //ICONS
    mActBooksTab->setIcon(EIcon::icon(BOOKS_TAB));
    mActScanFolder->setIcon(EIcon::icon(WIZARD));
    mActOpen->setIcon(EIcon::icon(OPEN));
    mActOpenWith->setIcon(EIcon::icon(OPEN));
    mActMoveToAuthor->setIcon(EIcon::icon(AUTORS));
    mActMoveToGroup->setIcon(EIcon::icon(GROOPS));
    mActConfig->setIcon(EIcon::icon(CONFIGURE));
    mActAddBook->setIcon(EIcon::icon(OPEN));
    mActAddCurrent->setIcon(EIcon::icon(ADD));
    mActShowFullScreen->setIcon(EIcon::icon(FULL_SCREEN));
    actBookmark->setIcon(EIcon::icon(BOOKMARK));

    //Checkable
    mActShowFullScreen->setCheckable(true);
    mActShowFullScreen->setChecked(isFullScreen());
    mActStandardMode->setCheckable(true);


    //View Mode Actions
    int vMode=setting.value("ViewMode",0).toInt();

    ui->stackedWidget->setCurrentIndex(vMode);

    // Category Actions
    int cat=setting.value("Category",0).toInt();
    mModel->setCategory(cat);

    //recent
    menuRecent=new QMenu(tr("Recent files"),this);
    mActRecents=new QAction(EIcon::icon(RECENT),tr("Recent files"),this);
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
        menuRecent->addAction(recentFileActs[i]);
    }

    mActRecents->setMenu(menuRecent);

    //Cnnections
    connect(mActRecents,SIGNAL(triggered()),recentFileActs[0],SLOT(trigger()));
    connect(mActBooksTab,SIGNAL(triggered()),this,SLOT(showTabBooks()));
    connect(mActAddBook,SIGNAL(triggered()),this,SLOT(addBook()));
    connect(mActAddCurrent,SIGNAL(triggered()),this,SLOT(addCurrentBook()));
    connect(mActOpen,SIGNAL(triggered()),this,SLOT(slotOpen()));
    connect(mActOpenWith,SIGNAL(triggered()),this,SLOT(slotOpenWith()));
    connect(mActScanFolder,SIGNAL(triggered()),this,SLOT(scanFolder()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(setCurentTab(int)));
    connect(mActMoveToAuthor, SIGNAL(triggered()),this, SLOT(changeAuthor()));
    connect(mActMoveToGroup, SIGNAL(triggered()),this, SLOT(changeGroup()));
    connect(mActAbout, SIGNAL(triggered()),this, SLOT(slotAbout()));
    connect(mActConfig, SIGNAL(triggered()),this, SLOT(configure()));
    connect(mActShowFullScreen, SIGNAL(triggered(bool)),this, SLOT(slotShowFullScreen(bool)));
    connect(mActStandardMode,SIGNAL(triggered(bool)),this,SLOT(switchToolBarMode(bool)));
    connect(mActFind,SIGNAL(triggered()),this,SLOT(showHideSearch()));
    connect(mActcleanBooks,SIGNAL(triggered()),mModel,SLOT(cleanBooksDir()));

    //menu file
    ui->menuFile->addAction(mActAddBook);
     ui->menuFile->addAction(mActAddCurrent);
    ui->menuFile->addAction(mActScanFolder);
    ui->menuFile->addAction(mActBooksTab);
    ui->menuFile->addSeparator();
    ui->menuFile->addMenu(menuRecent);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(mActConfig);
    ui->menuFile->addAction(actBookmark);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(EIcon::icon(QUITE),tr("&Quit"),this,SLOT(close()));
    //menu edit
    ui->menuEdit->addAction(mActFind);
    ui->menuEdit->addActions( mActions->listEditBooks());
    ui->menuEdit->addSeparator();
    ui->menuEdit->addActions( mActions->listCatBooks());
    ui->menuEdit->addSeparator();
    ui->menuEdit->addActions( mActions->listEditPdf());
    ui->menuEdit->addSeparator();
     ui->menuEdit->addAction(mActcleanBooks);

    //menu view

    ui->menuView->addActions(mActions->listViewBooks());
    ui->menuView->addActions(mActions->listViewPdf());
    ui->menuView->addSeparator();
    ui->menuView->addAction(mActShowFullScreen);
    ui->menuView->addAction(mActStandardMode);

    //menu help
    ui->menu_Help->addAction(mActAbout);
    QMenu *menuLang=ui->menu_Help->addMenu(tr("Switch Language..."));
    QActionGroup *actAlignLang=new QActionGroup(this);

    QSettings settings;
    settings.beginGroup("Main");
    QString locale= settings.value("Lang", "Default").toString();
    settings.endGroup();

    QAction *a=menuLang->addAction(tr("Default"));
    a->setCheckable(true);
    a->setData("Default");
    a->setChecked(locale=="Default");
    actAlignLang->addAction(a);

    a=menuLang->addAction(tr("Arabic"));
    a->setCheckable(true);
    a->setData("ar");
    a->setChecked(locale=="ar");
    actAlignLang->addAction(a);

    a=menuLang->addAction(tr("English"));
    a->setCheckable(true);
    a->setChecked(locale=="en");
    a->setData("en");
    actAlignLang->addAction(a);

    a=menuLang->addAction(tr("Francais"));
    a->setCheckable(true);
    a->setChecked(locale=="fr");
    a->setData("fr");
    actAlignLang->addAction(a);

    connect(actAlignLang,SIGNAL(triggered(QAction*))
            ,this,SLOT(swichLanguage(QAction*)));

    //tool bar

    // ui->mainToolBar->addAction(mActBooksTab);

    //  ui->mainToolBar->addAction(actBookmark);

    switchSkins();

    updateRecentFileActions();
    //QWidgetAction *wAct=new QWidgetAction(this);

}

void MainWindow::slotShowFullScreen(bool show)
{
qDebug()<<"fuul";
    if(show){
        mActShowFullScreen->setData(saveGeometry());

       showFullScreen();
       // mActShowFullScreen->setShortcut(QKeySequence(Qt::es));

    }else{

        showNormal();
       restoreGeometry(mActShowFullScreen->data().toByteArray());


    }
}

void MainWindow::switchSkins()
{

   // ui->mainToolBar->addAction(mActRecents);
    QSettings s;
    bool standardTools=s.value("StandardTools",false).toBool();
    mActStandardMode->setChecked(standardTools);

    if(mDebug)
        qDebug()<<"MainWindow::switchSkins() standardTools:"<<standardTools;



    if(standardTools){
        ui->mainToolBar->addAction(mActRecents);
        ui->mainToolBar->addActions( mActions->listStandardToolBarBooks());
        ui->mainToolBar->addActions( mActions->listStandardToolBarPdf());
        ui->mainToolBar->addSeparator();
        ui->mainToolBar->addAction(ui->dockWidgetBookmark->toggleViewAction());
    }else{
        ui->menuBar->setVisible(false);
        ui->tabWidget->setIconSize(ui->mainToolBar->iconSize());

        QToolButton *menuButton=new QToolButton(this);
        menuButton->setIcon(EIcon::icon(MENU_LIST));
        menuButton->setIconSize(ui->mainToolBar->iconSize());
        QMenu *menu=new QMenu(this);
        //   menu->addMenu(ui->menuFile);
        menu->addMenu(ui->menuEdit);
        menu->addMenu(ui->menuView);
        menu->addMenu(ui->menu_Help);
        menu->addActions(ui->menuFile->actions());
        menuButton->setAutoRaise(true);
        menuButton->setPopupMode(QToolButton::InstantPopup);
        menuButton->setMenu(menu);
        menuButton->setShortcut(QKeySequence("Alt+M"));

        QToolBar *toolCorner=new QToolBar;
        toolCorner->addWidget(menuButton);
        toolCorner->addAction(mActRecents);
        toolCorner->addAction(ui->dockWidgetBookmark->toggleViewAction());
        toolCorner->addSeparator();
        toolCorner->addAction(mActions->goHome());
        toolCorner->addAction(mActions->editBookInfo());


         ui->mainToolBar->addSeparator();
        ui->mainToolBar->addActions( mActions->listToolBarBooks());
        ui->mainToolBar->addActions( mActions->listToolBarPdf());

        ui->tabWidget->setCornerWidget(ui->mainToolBar,Qt::TopRightCorner);
        ui->tabWidget->setCornerWidget(toolCorner,Qt::TopLeftCorner);


    }


}

//______________________________________________________________________
void MainWindow::setCurentTab(int arg)
{
    /*!
            جعل بعض الاوامر مرئية والاخرى غير مرئية
            في حالة تغيير التبويب بين عرض الكتب
            وعرض الكتاب
     */
    if(mDebug)qDebug()<<"MainWindow::setCurentTab(arg) :"<<arg;

mActAddCurrent->setEnabled(false);

    bool visible=ui->tabWidget->currentWidget()!=ui->tabBooks;

    //BooKs tools
    foreach (QAction *act, mActions->listAllBooksActions()) {
        act->setVisible(!visible);
    }
      //Pdf view tools
    foreach (QAction *act, mActions->listAllPdfActions()) {
        act->setVisible(visible);
    }


    if(visible){
        WidgetPdf   *wPdf=widgetPdf();
        if(wPdf){

            mActions->setSpinPages(wPdf->numPages(),wPdf->curPage());
            //  mActions->setPdfContentVisibleAct(wPdf->isContentVisible());
            mActions->setRubberBandAct(wPdf->isRubberBand());
            mActions->setFitSizeAct(wPdf->docFiteWith());
            mActions->setLayoutModeAct(wPdf->doctLayoutMode());
            mActions->setContinousModeAct(wPdf->isContinousMode());
            mActions->setInvertcolorsAct(wPdf->isInvertColors());
mActAddCurrent->setEnabled(mModel->idFromFile(wPdf->filePath())==-1);
        }
    }
    ui->tabWidget->setTabsClosable(ui->tabWidget->count()-1);
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    setWindowTitle(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
#else
    setWindowTitle(tr("Books Organizer")+"-"+ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
#endif
}

//______________________________________________________________________Add
void MainWindow::scanFolder()
{
    if(mDebug) qDebug()<<"MainWindow::scanFolder() ";


    /*!
     * مسح مجلد بحثا عن ملفات كتب
     */

    AddFolderWizard *wiz=new AddFolderWizard(mModel->groupList(),mModel->authorList(),this);

    if( wiz->exec()==QDialog::Accepted)
    {

        QList<BookInfo *> list=wiz->getListBook();

        QProgressDialog progress(tr("Create books information..."), tr("Cancel"), 0, list.count(), this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setCancelButton(0);
        progress.show();
        qApp->processEvents();
        int i=0;
        foreach (BookInfo *b, list) {
            i++;
            progress.setValue(i);
            mModel->addBook(b->title(),b->author(),b->group(),b->icon()
                            ,b->comment(),b->properties(),b->path());
            qApp->processEvents();

        }

    }

    delete wiz;
    mModel->refreshPath();

}

//______________________________________________________________________Add
bool MainWindow::addBook(const QString &file)
{
    if(mDebug) qDebug()<<"MainWindow::addBook() ";

//QString pp="/media/Data/Documents/pdf-linux/haker/004.pdf";
    AddBookWizard *wiz=new AddBookWizard(mModel->groupList(),
                                         mModel->authorList(),file,this);
bool bookAdded=false;

    if( wiz->exec()==QDialog::Accepted)
    {
        QMap<QString,QString>map=wiz->getNewBook();
        // qDebug()<<map;
        mModel->addBook(map["Name"],map["Author"],map["Group"],map["Icon"]
                ,map["Comment"],map["Properties"],map["Exec"]);

bookAdded=true;
        slotOpenFileName(map["Exec"]);
        mModel->refreshPath();
    }
     delete wiz;

return bookAdded;


}

//void MainWindow::savRating(int r,QString file)
//{


//    qDebug()<<"save"<<r<<file;
//    //mModel->saveRating(file,r);
//}

//______________________________________________________________________Edit
void MainWindow::editBookOrGroup()
{
    if(mDebug)qDebug()<<"MainWindow::editBookOrGroup() ";


    /*!
     * تحرير الكتاب المحدد
     */

    QModelIndex index=currentIndex();

    if(!index.isValid())return;
    int id=mModel->idFromIndex(index).toInt();
    EditBookDialog *dlg=new EditBookDialog(mModel->isCategory());
    QString title=mModel->titleFromIndex(index);
    dlg->setTitle(title);
    dlg->setIcon(QIcon(Path::thumbnailsDir()+mModel->pixmapNameFromIndex(index))/*mModel->item(index.row(),0)->icon()*/);
    qApp->processEvents();
    if(!mModel->isCategory()){
        dlg->setComment(mModel->commentFromIndex(index));
        dlg->setAuthor(mModel->authorList(),mModel->authorFromIndex(index));
        dlg->setPath(mModel->fileNameFromIndex(index));
        dlg->setGroup(mModel->groupList(),mModel->groupFromIndex(index));
        dlg->setTags(mModel->tagsFromIndex(index));
        dlg->setRating(mModel->favoriteFromIndex(index).toInt());
    }

    if(dlg->exec()==QDialog::Accepted){
        if(mModel->isCategory()){
            mModel->updateCategiry(title,dlg->title());
        }else{

            mModel->updateBook(id,dlg->title(),dlg->author()
                               ,dlg->group(),dlg->comment()
                               ,dlg->path(),dlg->rating(),dlg->tags());
        }
    }

    delete dlg;

}

void MainWindow::editBookInfo()
{
    if(mDebug)qDebug()<<"MainWindow::editBookInfo() ";

    if(!widgetPdf())
        return;
//    qDebug()<<"editBookInfo2";
    QString path=widgetPdf()->filePath();



    QHash<QString,QString>hash=mModel->indexFromFileName(path );


    EditBookDialog *dlg=new EditBookDialog(false,true);
    int id=hash.value("id","-1").toInt();

    qDebug()<<"hash id"<<id;
   // if(!hash ["icon"].isEmpty())
   QString iconName=Path::thumbnailsDir()+hash ["icon"];

    if(!QFile::exists(iconName))
    {
        widgetPdf()->saveLogoPixmap(iconName);
    }

    QString title=hash ["title"];
    if(title.isEmpty())title=QFileInfo(path).baseName();

    QString curpath=hash ["exec"];
    if(curpath.isEmpty())curpath=path;


    dlg->setTitle(title);
    dlg->setIcon(QIcon(Path::thumbnailsDir()+hash ["icon"]));
    dlg->setComment( hash ["comment"]);
    dlg->setAuthor(mModel->authorList(),hash ["author"]);
    dlg->setRating( hash ["favo"].toInt());
    dlg->setPath(curpath);

    QString name= hash ["group"];
    dlg->setGroup(mModel->groupList(), name!=QString()? name :tr("Unknown"));
    dlg->setTags( hash ["tag"]);

    dlg->setPropertiesModel(widgetPdf()->propertiesModel());
    connect(dlg,SIGNAL(getFontModel(EditBookDialog*)),this,SLOT(fontModel(EditBookDialog *)));
   //   dlg->setFontsModel(widgetPdf()->fontsModel());

    if(dlg->exec()==QDialog::Accepted)
    {
        if(chekForaddBook() && id!=-1){

                mModel->updateBook(id,dlg->title(),dlg->author()
                                   ,dlg->group(),dlg->comment()
                                   ,dlg->path(),dlg->rating(),dlg->tags());

                ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),dlg->title());

        }
    }

    delete dlg;

}
void MainWindow::fontModel(EditBookDialog *w)
{
  if(widgetPdf())
  w->setFontsModel(widgetPdf()->fontsModel());
}

//______________________________________________________________________Delete
void MainWindow::deletSelectedIndexes()
{
    if(mDebug)qDebug()<<"MainWindow::deletSelectedIndexes() ";


    QModelIndexList selectedIndexes;
    if (focusWidget() == ui->listView){
        selectedIndexes=ui->listView->selectionModel()->selectedIndexes();
    }else if (focusWidget() == ui->treeView){
        selectedIndexes=ui->treeView->selectionModel()->selectedIndexes();
    }
    //  qDebug()<<selectedIndexes.count();

    mModel->deleteSelectedIndexes(selectedIndexes);

}

//______________________________________________________________________
void MainWindow::changeAuthor()
{
    if(mDebug)qDebug()<<"MainWindow::changeAuthor() ";

    moveItems(true);

}

void MainWindow::changeGroup()
{
    if(mDebug)qDebug()<<"MainWindow::changeGroup() ";

    moveItems(false);
}

void MainWindow::moveItems(bool authors)
{
    if(mDebug)qDebug()<<"MainWindow::moveItems() ";

    QModelIndexList selectedIndexes;
    if (focusWidget() == ui->listView){
        selectedIndexes=ui->listView->selectionModel()->selectedIndexes();
    }else if (focusWidget() == ui->treeView){
        selectedIndexes=ui->treeView->selectionModel()->selectedIndexes();
    }
    if(selectedIndexes.count()<1)return;

    QStringList items;
    QString name;
    if(authors){
        items=mModel->authorList();
        name=tr("Author");
    }else{
        items=mModel->groupList();
        name=tr("Group");
    }

    bool ok;
    QString item = QInputDialog::getItem(this, tr(""),
                                         tr("Select %1:").arg(name), items, -1, true, &ok);
    if (!ok || item.isEmpty())return;

    QList<int>list;
    QProgressDialog progress(tr("Change informations..."), tr("Cancel"), 0
                             , selectedIndexes.count()/5, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setCancelButton(0);
    progress.show();
    qApp->processEvents();
    int i=0;
    foreach (QModelIndex index, selectedIndexes)
    {

        if(!list.contains( index.row())){
            list.append(index.row());
            i++;
            progress.setValue(i);

            mModel->changeParent(index,item,authors);

            qApp->processEvents();
        }
    }

    mModel->refreshPath();

}

//______________________________________________________________________
void MainWindow::goRootPath()
{
    if(mDebug)qDebug()<<"MainWindow::goRootPath() ";

    mModel->setCategory(mModel->category());
    ui->treeView->resizeColumnToContents(0);

}

//__________________________________________________________________open
void MainWindow::slotItemActivated(const QModelIndex &index)
{
    if(mDebug)qDebug()<<"MainWindow::slotItemActivated() ";


    if(mModel->isRoot(index)){

        QString title=mModel->titleFromIndex(index);
        mModel->openCategory(title);

    }else{

        QString fileName=mModel->fileNameFromIndex(index);
        slotOpenFileName(fileName);

    }
mModel->sort(0);


}

//__________________________________________________________________open
void MainWindow::slotOpen()
{
    if(mDebug)qDebug()<<"MainWindow::slotOpen() ";


    QModelIndex index=currentIndex();
    if(index.isValid())
        slotItemActivated(index);

}

//__________________________________________________________________open
void MainWindow::slotOpenWith()
{
if(mDebug)qDebug()<<"MainWindow::slotOpenWith() ";

    QModelIndex index=currentIndex();
    if(!index.isValid())return;

    QString fileName=mModel->fileNameFromIndex(index);
    QFileInfo fi(fileName);
    if(QFile::exists(Path::booksDir()+fi.fileName()))
       fileName=Path::booksDir()+fi.fileName();

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

}

//__________________________________________________________________open
void MainWindow::openRecentFile()
{
    if(mDebug)qDebug()<<"MainWindow::openRecentFile() ";

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        slotOpenFileName(action->data().toString());

}

//__________________________________________________________________open
void MainWindow::slotOpenFileName(const QString &fileName)
{

    if (!isActiveWindow())
        activateWindow();

    if(mDebug)qDebug()<<"MainWindow::slotOpenFileName(fileName) :"<<fileName;

    QString mFileName=fileName;

    QFileInfo fi(fileName);

    // check if file exists in working dir
    if(QFile::exists(Path::booksDir()+fi.fileName()))
        mFileName=Path::booksDir()+fi.fileName();


    //check if book is open{ed
    for (int i = 0; i < ui->tabWidget->count(); ++i)
    {

        WidgetPdf   *w= qobject_cast<WidgetPdf *>( ui->tabWidget->widget(i));
        if(w)
        {

            if(w->filePath()==mFileName){
                qDebug()<<"MainWindow::slotOpenFileName() file is opened:" << mFileName ;
                ui->tabWidget->setCurrentWidget(w);

                return;
            }

        }

    }

    //check if book is added to model
//    if(mModel->idFromFile(fileName)==-1)
//    {
//        if(addBook(fileName))
//            return;
//    }

    //open this book
    WidgetPdf    *widgetPdf=new WidgetPdf;
    int page=mModel->page(fileName);

    if(widgetPdf->openDoc(mFileName,page))
    {
        qDebug()<<"MainWindow::slotOpenFileName() :" << mFileName ;
        ui->tabWidget->addTab(widgetPdf,EIcon::icon(APP_PDF)/*QIcon(":/icons/application-pdf.png")*/,  bookTitle(mFileName/*,widgetPdf*/));

        ui->tabWidget->setCurrentWidget(widgetPdf);
        connect(widgetPdf,SIGNAL(pageChanged(QString,int)),mModel,SLOT(savePage(QString,int)));
        connect(widgetPdf,SIGNAL(curPageChanged(int)),this,SIGNAL(curPageChanged(int)));

        connect(widgetPdf,SIGNAL(addNewBookmark(QString,int,qreal,qreal,QString)),
                mWidgetBookmark,SLOT(addBookmark(QString,int,qreal,qreal,QString)));


    }
    else
    {

        delete widgetPdf;

    }


}

//________________________________________________________________________
QString MainWindow::bookTitle(const QString &fileName/*,WidgetPdf *w*/)
{
    if(mDebug)qDebug()<<"MainWindow::bookTitle(fileName) :"<<fileName;

    QString title=mModel->titleFromFile(fileName);

//    if(title.isEmpty()&& w){

//        QMap<QString,QString>map=w->docInfo();

//        mModel->addBook(map["Name"],map["Author"],map["Group"],map["Icon"]
//                ,map["Comment"],map["Properties"],map["Exec"]);

//        mModel->refreshPath();

//        title=map["Name"];

//    }

    if(title.isEmpty()){
        title=QFileInfo(fileName).baseName();
    }

    if(title.length()>40){
        title= title.left(37);
        title=title.leftJustified(40,'.');
    }

    //save recent
    QSettings settings;
    settings.sync();
    QStringList files = settings.value("RecentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("RecentFileList", files);
    updateRecentFileActions();

    return title;

}

//____________________________________________________________________________________________________
void MainWindow::slotcurrentRowChanged(const QModelIndex & current, const QModelIndex & /*previous*/ )
{
    if(mDebug)qDebug()<<"MainWindow::slotcurrentRowChanged() ";

    if(mModel->isRoot(current))
    {

        //        ui->labelStars->setPixmap(QPixmap());
        ui->labelImageInfo->setPixmap(mModel->item(current.row(),0)->icon().pixmap(256));
        ui->labelCommentInfo->setText(QString());
        ui->labelAuthorInfo->setText(QString());
        ui->textEditPrprtiesInfo->setText(QString());
        ui->labelTags->setText(QString());
        mRatingWidget->clearRating();
        mRatingWidget->setVisible(false);
    }
    else
    {

        int rating=mModel->favoriteFromIndex(current).toInt();

        mRatingWidget->setVisible(true);

        mRatingWidget->setRating(rating,mModel->fileNameFromIndex(current));

        QString iconName=Path::thumbnailsDir()+mModel->pixmapNameFromIndex(current);

         QPixmap pix(iconName);
        ui->labelImageInfo->setPixmap(pix);

        ui->labelCommentInfo->setText(mModel->commentFromIndex(current));
        ui->labelAuthorInfo->setText(mModel->authorFromIndex(current));
        QString prop=mModel->propertiesFromIndex(current);
        prop.replace("CreationDate",tr("Created"));
        prop.replace("Title",tr("Title"));
        prop.replace("Author",tr("Author"));
        prop.replace("Creator",tr("Creator"));
        prop.replace("Producer",tr("Producer"));
        prop.replace("ModDate",tr("Modified"));
        prop.replace("Format",tr("Format"));
        prop.replace("Pages",tr("Pages"));
        prop+="\n"/*+tr("File:")*/+mModel->fileNameFromIndex(current);
        ui->textEditPrprtiesInfo->setText(prop);

        QString tags=mModel->tagsFromIndex(current);
        QStringList list=tags.split(",");
        QString txt=tr("Tag:");
        foreach (QString  s, list) {
            txt+=       QString( "<a href=\"%1\"><span >%1</span></a><span>  </span>").arg(s);
            //qDebug()<<"id :<<<<"<<mModel->idFromIndex(current);
        }



        ui->labelTags->setText(txt);
    }


    QString title=mModel->titleFromIndex(current);
    ui->labelTitleInfo->setText(title);

    ui->statusBar->showMessage(mModel->pathToString()+"/"+title);
}

//______________________________________________________________________
void MainWindow::customContextMenu(QPoint)
{
    //********DEBUG************
    if(mDebug)qDebug()<<"MainWindow::customContextMenu() ";

    QModelIndex index=currentIndex();
    int count=0;
    if (focusWidget() == ui->listView){
        count=ui->listView->selectionModel()->selectedIndexes().count();
    }else if (focusWidget() == ui->treeView){
        count=ui->treeView->selectionModel()->selectedIndexes().count();
    }

    QMenu menu;
    menu.addAction(mActions->goHome());
     menu.addSeparator();
    if(count>0&&index.isValid())
    {

        menu.addAction(mActOpen);
        if(!mModel->isRoot(index))
            menu.addAction(mActOpenWith);

        menu.addSeparator();
        //        menu.addAction(mActEditBook);
        //        menu.addAction(mActDelete);
        menu.addActions(    mActions->listEditBooks());
        if(!mModel->isRoot(index)){
            menu.addAction(mActMoveToAuthor);
            menu.addAction(mActMoveToGroup);
        }



    }

    menu.addSeparator();
    menu.addAction(mActAddBook);
    menu.addAction(mActAddCurrent);
    menu.exec(QCursor::pos());

}

//______________________________________________________________________
void MainWindow::toggleView(QAction* action)
{
    //********DEBUG************
    if(mDebug)qDebug()<<"MainWindow::toggleView() ";

    int mode=action->data().toInt();
    ui->stackedWidget->setCurrentIndex(mode);
    QSettings setting;
    setting.setValue("ViewMode",mode);
}

//______________________________________________________________________
void MainWindow::toggleRootPath(QAction* action)
{
    //********DEBUG************
    if(mDebug)qDebug()<<"MainWindow::toggleRootPath() ";

    int cat=action->data().toInt();
    mModel->setCategory(cat);
    QSettings setting;
    setting.setValue("Category",cat);

}

//______________________________________________________________________
QModelIndex MainWindow::currentIndex()
{
    //********DEBUG************
    if(mDebug)qDebug()<<"MainWindow::currentIndex() ";

    QModelIndex index;
    if (focusWidget() == ui->listView){
        index=ui->listView->currentIndex();
    }else if (focusWidget() == ui->treeView){
        index=ui->treeView->currentIndex();
    }
    return index;
}


//________________________________________________________Search
void MainWindow::goSearch(const QString &arg)
{
    //********DEBUG************
    if(mDebug)qDebug()<<"MainWindow::goSearch(arg) "<<arg;

    mModel->searchTitle(arg,mSearchTools->isMachCase());
}

//_________________________________________________________recent
void MainWindow::updateRecentFileActions()
{
    //********DEBUG************
    if(mDebug)qDebug()<<"MainWindow::updateRecentFileActions() ";

    QSettings settings;;

    settings.sync();
    QStringList files = settings.value("RecentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString name=mModel->titleFromFile(files[i]);
        name=name!=QString() ? name:QFileInfo(files[i]).baseName();
        QString text = tr("&%1 %2").arg(i + 1).arg(name);
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);

    }

    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

}

//_________________________________________________________dragdrop
void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    //********DEBUG************
    if(mDebug)qDebug()<<"MainWindow::dragEnterEvent() ";
    //********DEBUG************

    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();

    }


}

//_________________________________________________________dragdrop
void MainWindow::dropEvent(QDropEvent *e)
{
    //********DEBUG************
    if(mDebug)qDebug()<<"MainWindow::dropEvent() ";
    //********DEBUG************

    /*!
           * فتح الكتاب في حالة البيانات كانت عبارة عن
           * ملفات ادلة
           * ثم فتح الملف الاول فقط عند تحقق شرط كونها مافات كتب بلاحقة
           * pdf
           */

    const QMimeData *mime = e->mimeData();

    QList<QUrl> duplicates;

    foreach (QUrl url, mime->urls())
    {

        if (duplicates.contains(url))
            continue;
        else
            duplicates << url;

        qDebug()<<"MainWindow::dropEvent() :" <<url;
        QString fileName(url.toLocalFile());
        QFileInfo fi(fileName);

        if (fi.exists() && fi.suffix().toLower()=="pdf" )
        {

            qDebug()<<"MainWindow::dropEvent() :" << fileName << fi.exists() << fi.suffix().toLower();
            slotOpenFileName(fileName);
            return;

        }


    }

}

//________________________________________________________________________
void MainWindow::slotAbout()
{
    QMessageBox::about(this, tr("About booksorg"),
                       tr("<p><b>booksorg %1</b></p><p>booksorg is a easy organizer for your PDF books "
                          "using  poppler library.<p> See <a href=\"https://elkirtasse.sourceforge.net/\">elkirtasse.sourceforge.net</a> for more information.</p><p>&copy; About Zakaria</p>").arg(QApplication::applicationVersion()));

}

void MainWindow::switchToolBarMode(bool arg)
{
    QSettings s;
    s.setValue("StandardTools",arg);
    QMessageBox::information(this,"",tr(
                                 "The change will take effect the next time the application is started."));
}

//________________________________________________________________________
void MainWindow::swichLanguage(QAction* act)
{

    QSettings settings;
    settings.beginGroup("Main");
    settings.setValue("Lang",act->data().toString());
    settings.endGroup();
    QMessageBox::information(this,"",tr("The language for this application has been changed.\n"
                                        "The change will take effect the next time the application is started."));



}

//________________________________________________________________________
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    //********DEBUG************
    if(mDebug)qDebug()<<"MainWindow::on_tabWidget_tabCloseRequested(index) "<<index;
    //********DEBUG************

    if( ui->tabWidget->indexOf(ui->tabBooks)==index){

        ui->tabWidget->removeTab(index);

    }else{

        //        WidgetPdf   *w= qobject_cast<WidgetPdf *>( ui->tabWidget->widget(index));
        //        if(w)
        delete ui->tabWidget->widget(index);

    }

    ui->tabWidget->setTabsClosable(ui->tabWidget->count()-1);

}

//________________________________________________________________________
void MainWindow::slotAddBookmark()
{
    if(mDebug)qDebug()<<"MainWindow::slotAddBookmark() ";

    // WidgetPdf   *w= qobject_cast<WidgetPdf *>( ui->tabWidget->currentWidget());
    if(widgetPdf())
        widgetPdf()->addBookmark();

}

//________________________________________________________________________
void MainWindow::bookmarkOpenDocument(const QString &fileName,int page,qreal left,qreal top)
{
    if(mDebug)qDebug()<<"MainWindow::bookmarkOpenDocument(fileName , page) :"<<fileName<<page;

    slotOpenFileName(fileName);
    //  WidgetPdf   *w= qobject_cast<WidgetPdf *>( ui->tabWidget->currentWidget());
    if(widgetPdf())
        widgetPdf()->jumpToPage(page,left,top);

}

//____________________________PDFWIDGET______________________________________
WidgetPdf *MainWindow::widgetPdf()
{
    WidgetPdf   *w= qobject_cast<WidgetPdf *>( ui->tabWidget->currentWidget());
    if(w)
        return w;
    return 0;
}

void MainWindow::wPdfEmitFirstPage()
{
    if(mDebug)qDebug()<<"MainWindow::wPdfEmitFirstPage() ";

    if(widgetPdf())
        widgetPdf()->docEmitFirstPage();
}
void MainWindow::wPdfEmitPreviousPage()
{
    if(mDebug)qDebug()<<"MainWindow::wPdfEmitPreviousPage() ";

    if(widgetPdf())
        widgetPdf()->docEmitPreviousPage();
}
void MainWindow::wPdfEmitNextPage()
{
    if(mDebug)qDebug()<<"MainWindow::wPdfEmitNextPage() ";

    if(widgetPdf())
        widgetPdf()->docEmitNextPage();
}
void MainWindow::wPdfEmitLastPage()
{
    if(mDebug)qDebug()<<"MainWindow::wPdfEmitLastPage() ";

    if(widgetPdf())
        widgetPdf()->docEmitLastPage();
}
void MainWindow::wPdfEmitAddBookmark()
{
    if(mDebug)qDebug()<<"MainWindow::wPdfEmitAddBookmark() ";

    if(widgetPdf())
        widgetPdf()->docEmitAddBookmark();

}
void MainWindow::wPdfRefresh()
{
    if(mDebug)qDebug()<<"MainWindow::wPdfRefresh() ";

    if(widgetPdf())
        widgetPdf()->docRefresh();
}
void MainWindow::wPdfEmitZoomIn()
{
    if(mDebug)qDebug()<<"MainWindow::wPdfEmitZoomIn() ";

    if(widgetPdf())
        widgetPdf()->docEmitZoomIn();
}
void MainWindow::wPdfEmitZoomOut()
{
    if(mDebug)qDebug()<<"MainWindow::wPdfEmitZoomOut() ";

    if(widgetPdf())
        widgetPdf()->docEmitZoomOut();
}
void MainWindow::wPdfEmitSetRubberBand(bool arg)
{
    if(mDebug)qDebug()<<"MainWindow::wPdfEmitSetRubberBand() "<<arg;

    if(widgetPdf())
        widgetPdf()->docEmitSetRubberBand(arg);
}
void MainWindow::wPdfEmitSetInvertColors(bool arg)
{
    if(mDebug)qDebug()<<"MainWindow::wPdfEmitSetInvertColors() "<<arg;

    if(widgetPdf())
        widgetPdf()->docEmitSetInvertColors(arg);
}

void MainWindow::wPdfSetPage(int arg)
{
    if(mDebug)qDebug()<<"MainWindow::wPdfSetPage() "<<arg;

    if(widgetPdf())
        widgetPdf()->docSetPage(arg);
}

void MainWindow::wPdfSetContinousMode(bool arg)
{
    if(mDebug)qDebug()<<"MainWindow::wPdfSetContinousMode() "<<arg;

    if(widgetPdf())
        widgetPdf()->docSetContinousMode(arg);



}

void MainWindow::wPdfSetFiteWith(int arg)
{
    if(mDebug)qDebug()<<"MainWindow::wPdfSetFiteWith() "<<arg;

    if(widgetPdf())
        widgetPdf()->docSetFiteWith(arg);


}

void MainWindow::wPdfSetLayoutMode(int arg)
{
    if(mDebug)qDebug()<<"MainWindow::wPdfSetLayoutMode() "<<arg;

    if( widgetPdf())
        widgetPdf()->docSetLayoutMode(arg);

}

//_______________________________________________________   SEARCH
void MainWindow::showHideSearch()
{
    if(mDebug)qDebug()<<"MainWindow::showHideSearch() ";

    if(ui->tabWidget->currentWidget()==ui->tabBooks){
        if(mSearchTools->isHidden()){
            mSearchTools->setVisible(true);
            mSearchTools->show();
        }else
            mSearchTools->hide();

    }
    else if(widgetPdf())
        widgetPdf()->showHideSearch();
}

//_______________________________________________________________TABS
void MainWindow::showTabBooks()
{
    if(mDebug)qDebug()<<"MainWindow::showTabBooks() ";

    int i=  ui->tabWidget->indexOf(ui->tabBooks);
    if(i<0){

        ui->tabWidget->addTab(ui->tabBooks,EIcon::icon(BOOKS_TAB),tr("Books"));
        ui->tabWidget->setCurrentWidget(ui->tabBooks);
    }else{
        ui->tabWidget->setCurrentIndex(i);
    }

}

//__________________________________________________________________TAGS
void MainWindow::on_labelTags_linkActivated(const QString &link)
{
    if(mDebug)qDebug()<<"MainWindow::on_labelTags_linkActivated() "<<link;

    // QMessageBox::information(this,"",link);
    mModel->   searchTags(link);
}

//_________________________________________________________________CONFIGURE
void MainWindow::configure()
{
    if(mDebug)qDebug()<<"MainWindow::configure() ";

    ConfigureDialog *dlg=new ConfigureDialog;

    if(dlg->exec()==QDialog::Accepted){
        qDebug()<<"MainWindow::configure() ";
        QSettings settings;
        settings.sync();

        //         mModel->setBooksPath(settings.value("BooksPath",
        //                      QDir::homePath()+"/."+QApplication::applicationName()).toString());

        Path::setCopyBooks(settings.value("CopyBooks",false).toBool());
        QString  newPath=settings.value("BooksPath",
                                        QDir::homePath()+"/."+QApplication::applicationName()).toString();


        if(Path::workingDir()!=newPath){

            QDir dir(newPath);
            dir.mkdir(newPath+"/books");
            dir.mkdir(newPath+"/thumbnails");

            Path::setWorkingDir(newPath);

        }
        mModel->updateDataBase();
        //qDebug()<<"MainWindow::configure() "<<Path::workingDir();
    }
}

void MainWindow::updateItem(QModelIndex index)
{
    if(focusWidget()==ui->listView)
        ui->listView->update(index);
    else  if(focusWidget()==ui->treeView)
        ui->treeView->update(index);
}

// adding book if no exist
bool MainWindow::chekForaddBook()
{
    if(!widgetPdf())
        return false;
//    qDebug()<<"editBookInfo2";
    QString path=widgetPdf()->filePath();

    //check if book is added to model
    if(mModel->idFromFile(path)>-1)
    {

            return true;
    }


    QMessageBox msgBox;
    msgBox.setText(tr("The book no existes."));
    msgBox.setInformativeText("Do you want to add the book?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No) ;
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    if(ret==QMessageBox::Yes )
    {
        if (addBook(path))
         return true;
    }

    return false;

}

void MainWindow::addCurrentBook()
{
    if(!widgetPdf())
        return ;
    QString path=widgetPdf()->filePath();

    //check if book is added to model
    if(mModel->idFromFile(path)>-1)
               return ;

    addBook(path);
}
