#include "widgetbookmark.h"
#include <QVBoxLayout>
#include<QHeaderView>
#include<QMessageBox>
#include<QInputDialog>
#include<QMenu>
#include "eicon.h"
#include "addbookmarkdlg.h"

WidgetBookmark::WidgetBookmark(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout* vLayoutBookmark = new QVBoxLayout(this);
    vLayoutBookmark->setSpacing(3);
    vLayoutBookmark->setContentsMargins(3, 3, 3, 3);

    //QToolBar
    mToolBarBookmark=new QToolBar;
    vLayoutBookmark->insertWidget(1,mToolBarBookmark);

    //QTreeWidget
    treeWidgetBookmark = new QTreeWidget(this);
    treeWidgetBookmark->header()->setVisible(false);
    vLayoutBookmark->addWidget(treeWidgetBookmark);

    //QAction
    actAddBookmark= new QAction(EIcon::icon(BOOKMARK_ADD),tr("&Add Bookmark"),this);
    actAddBookmark->setShortcut(QKeySequence("Ctrl+B"));
    connect(actAddBookmark,SIGNAL(triggered()),this,SIGNAL(getBookmark()));

    QAction *actRemoveBookmark=new QAction(EIcon::icon(REMOVE),tr("Remove Bookmark"),this) ;
    connect(actRemoveBookmark,SIGNAL(triggered()),this,SLOT(removeBookmark()));
    QAction *actRenameBookmark=new QAction(EIcon::icon(RENAME),tr("Rename Bookmark"),this) ;
    connect(actRenameBookmark,SIGNAL(triggered()),this,SLOT(renameBookmark()));

    mToolBarBookmark->addAction(actAddBookmark);
    mToolBarBookmark->addAction(actRenameBookmark);
    mToolBarBookmark->addSeparator();
    mToolBarBookmark->addAction(actRemoveBookmark);

    //Load Settings
    loadBookmarkSettings();

    //Connection
    treeWidgetBookmark->setContextMenuPolicy(Qt::CustomContextMenu	);
    connect(treeWidgetBookmark,SIGNAL(customContextMenuRequested(QPoint))
            ,this,SLOT(customMenuBookmark(QPoint)));
    connect(treeWidgetBookmark,SIGNAL(itemActivated(QTreeWidgetItem*,int))
            ,this,SLOT(treeWidgetBookmarkItemActivated(QTreeWidgetItem*,int)));

    //----------------------------------------------


}

//____________________________________________________________Bookmark
void WidgetBookmark::addBookmark(const QString& path,int page,qreal left,qreal top,const QString &str)
{

    QStringList list;
    int count=treeWidgetBookmark->topLevelItemCount();

    for (int i = 0; i < count; ++i)
    {
        QTreeWidgetItem *item= treeWidgetBookmark->topLevelItem(i);
        list<<item->text(0);
    }

    if(list.isEmpty())
        list<<tr("New Group");

    AddBookmarkDlg *dlg=new AddBookmarkDlg(list);
    dlg->setBookmarkName(str);
    if(dlg->exec()==QDialog::Accepted)
    {

        QString name=dlg->getName();
        QString group=dlg->getGroup();
        if(!name.isEmpty() && !group.isEmpty())
        {

            bool exist=false;
            QTreeWidgetItem *childitem=new QTreeWidgetItem();
            childitem->setText(0,name);
            childitem->setData(0, Qt::UserRole + 1,page);
            childitem->setData(0, Qt::UserRole + 2,left);
            childitem->setData(0, Qt::UserRole + 3,top);
            childitem->setData(0, Qt::UserRole + 4,path);

            for (int i = 0; i < treeWidgetBookmark->topLevelItemCount(); ++i)
            {
                QTreeWidgetItem *item= treeWidgetBookmark->topLevelItem(i);
                if(item->text(0)==group){
                    item->addChild(childitem);
                    exist=true;
                    break;
                }
            }

            if(!exist){
                QTreeWidgetItem *item=new QTreeWidgetItem();
                item->setText(0,group);
                treeWidgetBookmark->addTopLevelItem(item);
                item->addChild(childitem);
            }

            treeWidgetBookmark->setCurrentItem(childitem);
            saveBookmarkSettings();
        }

    }

    delete dlg;


}
//___________________________________________________________Bookmark
void WidgetBookmark::saveBookmarkSettings()
{

    QSettings settings(QApplication::organizationName(),"BooksBookmarks") ;
    // REMOVE ALLKEYS
    foreach (QString s, settings.childGroups()) {
        settings.remove(s);
    }

    settings.sync();

    int count=treeWidgetBookmark->topLevelItemCount();

    for (int i = 0; i < count; ++i)
    {
        QTreeWidgetItem *topItem=treeWidgetBookmark->topLevelItem(i);
        if(!topItem)continue;


        settings.beginWriteArray(topItem->text(0));
        for (int r = 0; r < topItem->childCount(); ++r)
        {
            QTreeWidgetItem *item=topItem->child(r);
            if(!item)continue;

            settings.setArrayIndex(r);

            int page = item->data(0, Qt::UserRole + 1).toInt();
            qreal left =item->data(0, Qt::UserRole + 2).toReal();
            qreal top = item->data(0, Qt::UserRole + 3).toReal();
            QString file=item->data(0,Qt::UserRole+4).toString();

            settings.setValue("Name",item->text(0));
            settings.setValue("Page",page);
            settings.setValue("Left",left);
            settings.setValue("Top",top);
            settings.setValue("File",file);



        }

        settings.endArray();


    }


}

//___________________________________________________________Bookmark
void WidgetBookmark::loadBookmarkSettings()
{
    QSettings settings(QApplication::organizationName(),"BooksBookmarks") ;

    foreach (QString s, settings.childGroups())
    {

        QTreeWidgetItem *item=  new QTreeWidgetItem(treeWidgetBookmark,QStringList()<<s);

        int count = settings.beginReadArray(s);

        for (int i = 0; i < count; ++i)
        {
            settings.setArrayIndex(i);

            QString name=settings.value("Name").toString();
            int page = settings.value("Page").toInt();
            qreal left =settings.value("Left").toReal();
            qreal top = settings.value("Top").toReal();
            QString file=settings.value("File").toString();

            QTreeWidgetItem *childitem=new QTreeWidgetItem(item);

            childitem->setText(0,name);
            childitem->setData(0, Qt::UserRole + 1,page);
            childitem->setData(0, Qt::UserRole + 2,left);
            childitem->setData(0, Qt::UserRole + 3,top);
            childitem->setData(0, Qt::UserRole + 4,file);

        }

        settings.endArray();
    }
}

//____________________________________________________________Bookmark
void WidgetBookmark::treeWidgetBookmarkItemActivated(QTreeWidgetItem *item, int)
{
    if(!item->parent())
        return;

    int page = item->data(0, Qt::UserRole + 1).toInt();
    qreal left =item->data(0, Qt::UserRole + 2).toReal();
    qreal top = item->data(0, Qt::UserRole + 3).toReal();
    QString file=item->data(0,Qt::UserRole+4).toString();
    emit openDocument(file,page,left,top);


}
//____________________________________________________________Bookmark
void WidgetBookmark::removeBookmark()
{
    int ret=  QMessageBox::warning(0,"",tr("Do you want to delete selected bookmark")
                                   ,QMessageBox::Yes,QMessageBox::No);
    if(ret==QMessageBox::No)return;
    QTreeWidgetItem *item=treeWidgetBookmark->currentItem();
    if(!item)return;
    if(item->parent()){
        int index=item->parent()->indexOfChild(item);
        item->parent()->takeChild(index);
    }else{
        int index=treeWidgetBookmark->indexOfTopLevelItem(item);
        treeWidgetBookmark->takeTopLevelItem(index);

    }
    saveBookmarkSettings();
}
//____________________________________________________________Bookmark
void WidgetBookmark::renameBookmark()
{
    QTreeWidgetItem *item=treeWidgetBookmark->currentItem();
    if(!item)return;

    bool ok;
    QString text = QInputDialog::getText(this, "",
                                         tr("Enter new name:"), QLineEdit::Normal,
                                         item->text(0), &ok);
    if (ok && !text.isEmpty()){
        item->setText(0,text);
        saveBookmarkSettings();
    }
}
//___________________________________________________________
void WidgetBookmark::customMenuBookmark(QPoint)
{
    QMenu menu;

    menu.addActions(mToolBarBookmark->actions());
    menu.exec(QCursor::pos());

}
