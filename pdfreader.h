#ifndef PDFREADER_H
#define PDFREADER_H
#include "pdfwidget.h"
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>
#include <QSpinBox>
class PdfReader : public QWidget
{
    Q_OBJECT
public:
    explicit PdfReader(QWidget *parent = 0);
    
signals:

public slots:
 void openDoc(const QString &path);
private:
    QToolBar *mToolBarPdf;
    PdfWidget *pdfDoc;
    //اوامر
     QAction *mActionFitInwidth;    //! fitInwidthPdf()
     QAction *mActionNext;          //! nextPage()
     QAction *mActionPrev;          //! prevPage()
     QAction *mActionFirst;         //! firstPage()
     QAction *mActionLast;          //!  lastPage()
   QAction *mActionZoomIn;
     QAction *mActionZoomOut;
     QAction *mActionZoomOrig;
          QAction *mActionFitWitdh;

     QAction *mActionFind;
     QLineEdit *mLineEditFind;
QSpinBox *mSpinBox;
private slots:
     void setPdfPage(int index=-1 , QRectF rectF=QRectF(0,0,0,0));
     void firstPage();       /*!< للذهاب الى الصفحة الاولى */
     void nextPage();        /*!< تلذهاب الى الصفحة التالية */
     void prevPage();        /*!< للذهاب الى الصفحة السابقة */
     void lastPage();        /*!< للذهاب الى الصفحة الاخيرة */
void searchString();
void slotSetPage();
};

#endif // PDFREADER_H
