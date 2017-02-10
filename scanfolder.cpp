#include "scanfolder.h"
#include <poppler-qt4.h>
#include<QFileDialog>
#include<QDir>
#include<QDebug>
#include<QApplication>

#define BooksPath QDir::homePath()+"/."+QApplication::applicationName()+"/books"

#define thumbnails QDir::homePath()+"/."+QApplication::applicationName()+"/thumbnails"

ScanFolder::ScanFolder(QWidget *parent) :
    QWizard(parent)
{
     addPage(new ClassIntroScan);
     addPage(new ClassProgress);

}


ClassIntroScan::ClassIntroScan(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(tr("Path :"),this);

    horizontalLayout->addWidget(label);

    lineEdit = new QLineEdit(this);
    lineEdit->setReadOnly(true);
    horizontalLayout->addWidget(lineEdit);

    toolButton = new QToolButton(this);
    connect(toolButton,SIGNAL(clicked()),this,SLOT(openFolder()));
    horizontalLayout->addWidget(toolButton);

    label2 = new QLabel(tr("This wizard will generate informations "
                          "for pdf files "
                          "need to specify the folder ."
                          ));

    label2->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label2);
    layout->addLayout(horizontalLayout);
    setLayout(layout);
    registerField("dirPath*", lineEdit);
  }

void ClassIntroScan::openFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    lineEdit->setText(dir);

}


ClassProgress::ClassProgress(QWidget *parent)
    : QWizardPage(parent)
{

    setTitle(tr("Generate Files"));
    label = new QLabel(tr("Path :"),this);

    label2 = new QLabel(tr("This wizard will generate informations "
                          "for pdf files "
                          "need to specify the folder ."
                          ));

    label2->setWordWrap(true);
    progressBar=new QProgressBar;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(label2);
    layout->addWidget(progressBar);
    setLayout(layout);

  }

void ClassProgress::initializePage()
{
   mStop=false;
}
void ClassProgress::setVisible(bool visible)
 {
     QWizardPage::setVisible(visible);

     if (visible) {
         wizard()->setButtonText(QWizard::CustomButton1, tr("&Print"));
         wizard()->setOption(QWizard::HaveCustomButton1, true);
         connect(wizard(), SIGNAL(customButtonClicked(int)),
                 this, SLOT(creatPdfs()));
     } else {
         wizard()->setOption(QWizard::HaveCustomButton1, false);
         disconnect(wizard(), SIGNAL(customButtonClicked(int)),
                    this, SLOT(creatPdfs()));
     }
 }


void ClassProgress::creatPdfs()
{
      wizard()->setOption(QWizard::HaveCustomButton1, false);

    QString path = field("dirPath").toString();
    QDir dir(path);
    QStringList list=dir.entryList(QStringList()<<"*.pdf",QDir::Files);
    if(list.count()<1)return;
    wizard()->setOption(QWizard::DisabledBackButtonOnLastPage);

    progressBar->setRange(0,list.count());

    int i=0;
    foreach (QString file, list) {
        qApp->processEvents();
        i++;
        progressBar->setValue(i);
        qDebug()<<path+"/"+file;
        if(mStop)return;

        Poppler::Document   *doc = Poppler::Document::load(path+"/"+file);
        if (doc) {
            if(doc->numPages()>0){
                QImage image = doc->page(0)
                        ->renderToImage(1.0 * physicalDpiX(), 1.0 * physicalDpiY());

                QPixmap pix=QPixmap::fromImage(image).scaled(QSize(256,256),Qt::KeepAspectRatio);

            }

        }

    }
}
