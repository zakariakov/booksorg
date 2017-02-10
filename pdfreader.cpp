#include "pdfreader.h"
#include "pdfwidget.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QDebug>
PdfReader::PdfReader(QWidget *parent) :
    QWidget(parent)
{
    mToolBarPdf=new QToolBar(this);
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->setMargin(3);
    layout->addWidget(mToolBarPdf);
    pdfDoc=new PdfWidget;
    layout->addWidget(pdfDoc);
    mActionNext=new QAction(tr("Next Page"),this);
    mActionPrev=new QAction(tr("Previous Page"),this);
    mActionFirst=new QAction(tr("First Page"),this);
    mActionLast=new QAction(tr("Last Page"),this);
    mActionFind=new  QAction(QIcon::fromTheme("edit-find", QIcon(":/icons/edit-find.png")),
                             tr("Search"),this);

    mActionZoomIn=new  QAction(QIcon::fromTheme("zoom-in"),tr("Zoom In"),this);
    mActionZoomOut=new  QAction(QIcon::fromTheme("zoom-out"),tr("Zoom Out"),this);
    mActionZoomOrig=new  QAction(QIcon::fromTheme("zoom-original"),tr("Zoom Original"),this);
    mActionFitWitdh=new  QAction(QIcon::fromTheme("zoom-fit-page",QIcon::fromTheme("zoom-fit-best")),tr("Fit Width"),this);
    mActionFitWitdh->setCheckable(true);
   connect(mActionZoomIn,SIGNAL(triggered()),pdfDoc,SLOT(zoomIn()));
   connect(mActionZoomOut,SIGNAL(triggered()),pdfDoc,SLOT(zoomOut()));
   connect(mActionZoomOrig,SIGNAL(triggered()),pdfDoc,SLOT(zoomOriginal()));
   connect(mActionFitWitdh,SIGNAL(triggered(bool)),pdfDoc,SLOT(fitInwidthPdf(bool)));
   connect(pdfDoc,SIGNAL(fitWitdhChanged(bool)),mActionFitWitdh,SLOT(setChecked(bool)));


    mSpinBox=new QSpinBox;
    mSpinBox->setRange(0,0);

    mLineEditFind=new QLineEdit;
    mLineEditFind->setMaximumWidth(150);
    if(this->layoutDirection()==Qt::RightToLeft){
        mActionFirst->setIcon(QIcon::fromTheme("go-last", QIcon(":/icons/go-first.png")));
        mActionPrev->setIcon(QIcon::fromTheme("go-next", QIcon(":/icons/go-back.png")));
        mActionNext->setIcon(QIcon::fromTheme("go-previous", QIcon(":/icons/go-next.png")));
        mActionLast->setIcon(QIcon::fromTheme("go-first", QIcon(":/icons/go-last.png")));

    }else{
        mActionFirst->setIcon(QIcon::fromTheme("go-first", QIcon(":/icons/go-last.png")));
        mActionPrev->setIcon(QIcon::fromTheme("go-previous", QIcon(":/icons/go-next.png")));
        mActionNext->setIcon(QIcon::fromTheme("go-next", QIcon(":/icons/go-back.png")));
        mActionLast->setIcon(QIcon::fromTheme("go-last", QIcon(":/icons/go-first.png")));

    }
    //    mActionFitInwidth->setIcon(QIcon::fromTheme("zoom-fit-page", QIcon(":/images/image/zoom-fit-width.png")));

    connect(mActionFirst,SIGNAL(triggered()),this,SLOT(firstPage()));
    connect(mActionNext,SIGNAL(triggered()),this,SLOT(nextPage()));
    connect(mActionPrev,SIGNAL(triggered()),this,SLOT(prevPage()));
    connect(mActionLast,SIGNAL(triggered()),this,SLOT(lastPage()));
    connect(mLineEditFind,SIGNAL(returnPressed()),this,SLOT(searchString()));
    connect(mActionFind,SIGNAL(triggered()),this,SLOT(searchString()));
    connect(mSpinBox,SIGNAL(editingFinished()),this,SLOT(slotSetPage()));


    mToolBarPdf->addAction(mActionFirst);
    mToolBarPdf->addAction(mActionPrev);
    mToolBarPdf->addWidget(mSpinBox);
    mToolBarPdf->addAction(mActionNext);
    mToolBarPdf->addAction(mActionLast);


    mToolBarPdf->addSeparator();
    mToolBarPdf->addWidget(mLineEditFind);
    mToolBarPdf->addAction(mActionFind);
     mToolBarPdf->addSeparator();
     mToolBarPdf->addAction(mActionZoomIn);
     mToolBarPdf->addAction(mActionZoomOut);
     mToolBarPdf->addAction(mActionZoomOrig);
     mToolBarPdf->addAction(mActionFitWitdh);

     QList<QAction *> list;
     QAction *act=new QAction(this);
     act->setSeparator(true);
     list<<mActionNext<<mActionPrev<<mActionFirst<<mActionLast<<act<<mActionFitWitdh;
    pdfDoc->setListActions(list);
}
void PdfReader::openDoc(const QString &path)
{
    if(pdfDoc->setDocument(path)){
        mSpinBox->setRange(1,pdfDoc->pagesCount());
         mSpinBox->setValue(1);
    }

}

//! السجل الاول------------------------------------
void PdfReader::firstPage()//
{

        setPdfPage(1);

}
//! السجل التالي-----------------------------------
void PdfReader::nextPage()//
{

        int i=  pdfDoc->pageCurent();
        setPdfPage(i+1);

}
//! السجل السابق------------------------------------
void PdfReader::prevPage()//
{

        int i=  pdfDoc->pageCurent();
        setPdfPage(i-1);

}
//! السجل الاخير-------------------------------------
void PdfReader::lastPage()//
{

        setPdfPage(pdfDoc->pagesCount());
        return;

}

void PdfReader::setPdfPage(int index, QRectF rectF)
{
    if(!pdfDoc)return;
    int count=pdfDoc->pagesCount();
    if(count<1)return;
    if(index>count||index<1)return;

    pdfDoc->setPage(index,rectF);
    mSpinBox->setValue(index);
/*

    QList<QTreeWidgetItem *> found =
            treeViewFahras->findItems(
               QString::number(index),Qt::MatchExactly | Qt::MatchRecursive,1);//تنشاء قائمة بكل العناصر
    foreach (QTreeWidgetItem *item, found){
        treeViewFahras->setCurrentItem(item);
    }

   */
}
void PdfReader::slotSetPage()
{
    setPdfPage(mSpinBox->value());
}

void PdfReader::searchString()
{
    QString txt=mLineEditFind->text();

            if(txt.length()<3)return;
             //TODO:add searsh derection for qt5
            if(QApplication::keyboardInputDirection()==Qt::RightToLeft){
                QString str;
                for(int i=txt.length();i>-1;i--){
                    str+=txt.mid(i,1);
                   qDebug()<<str;
                }
                   pdfDoc->searchForwards(str);

            }else{
                 pdfDoc->searchForwards(txt);
            }
 qDebug()<<txt;

            /*
    if(QApplication::keyboardInputDirection()==Qt::RightToLeft){
        QString str;
        for(int i=txt.length();i>-1;i--){
            str+=txt.mid(i,1);
            //qDebug()<<str;
        }
           pdfDoc->searchForwards(str);

    }
    else //اذا كان الاتجاه من اليسار الى اليمين
      pdfDoc->searchForwards(txt);
      */
}
