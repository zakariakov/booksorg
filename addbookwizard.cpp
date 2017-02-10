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
#include "addbookwizard.h"
#include "bookinfo.h"

#include <QVBoxLayout>
#include <QFileInfo>
#include <QFileDialog>
//#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <QTextCodec>
#include <QInputDialog>

//#define BooksPath QDir::homePath()+"/."+QApplication::applicationName()+"/books"
//#define thumbnails QDir::homePath()+"/."+QApplication::applicationName()+"/thumbnails"

AddBookWizard::AddBookWizard(const QStringList &groups, const QStringList &autors,
                             const QString &file, QWidget *parent) :
    QWizard(parent),mGroups(groups),mAuthors(autors)
{
    setWindowTitle(tr("Add Book"));
    // addPage(new IntroPage);
    if(file.isEmpty()){

         addPage(new ClassInfoPage);
    }

    mOutputFilesPage=  new OutputFilesPage(this,file);
    addPage(mOutputFilesPage);
    resize(570,360);
}

void AddBookWizard::accept()
{
    QString path=mOutputFilesPage->getFilePath();/*field("filePath").toString();*/
    QString title=field("Title").toString();
    QString comment=field("Comment").toString();


    mMap.insert("Name",title.remove("'"));
    mMap.insert("Author",mOutputFilesPage->getAuthor().remove("'"));
    mMap.insert("Group",mOutputFilesPage->getGroup().remove("'"));
    mMap.insert("Icon",mPixName);
    mMap.insert("Comment",comment.remove("'"));
    mMap.insert("Properties",mOutputFilesPage->getInfo().remove("'"));
    mMap.insert("Exec",path);


      QDialog::accept();


}

ClassInfoPage::ClassInfoPage(QWidget *parent)
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
    toolButton->setText(tr("Open file"));
    toolButton->setIcon(QIcon::fromTheme("document-open",QIcon(":/icons/document-open")));

    connect(toolButton,SIGNAL(clicked()),this,SLOT(openFile()));

    horizontalLayout->addWidget(toolButton);
  //  setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label2 = new QLabel(tr("This wizard will generate a file information "
                          "for pdf file "
                          "need to specify the pdf file name ."
                          ));

    label2->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label2);
    layout->addLayout(horizontalLayout);
    setLayout(layout);
    registerField("filePath*", lineEdit);

}

void ClassInfoPage::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::homePath(),
                                                    tr("PDF document (*.pdf)"));

    if(!fileName.isEmpty()){
       lineEdit->setText(fileName);
    }

}
OutputFilesPage::OutputFilesPage(AddBookWizard *p, const QString &file, QWidget *parent)
    : QWizardPage(parent),mFile(file),mparent(p)
{
    setTitle(tr("Informations"));
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_2 = new QLabel(tr("Title"),this);
    gridLayout->addWidget(label_2, 0, 0, 1, 1);

    lineEditTitle = new QLineEdit(this);
    lineEditTitle->setObjectName(QString::fromUtf8("lineEditTitle"));

    gridLayout->addWidget(lineEditTitle, 0, 1, 1, 1);

    toolButtonImage = new QLabel(this);
    toolButtonImage->setMinimumSize(QSize(256, 256));
    // toolButtonImage->setIconSize(QSize(256,256));
    toolButtonImage->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(toolButtonImage, 0, 2, 5, 1);

    label_3 = new QLabel(tr("Comment :"),this);
    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    lineEditComment = new QLineEdit(this);
    gridLayout->addWidget(lineEditComment, 1, 1, 1, 1);

    label_5 = new QLabel(tr("Author :"),this);
    gridLayout->addWidget(label_5, 2, 0, 1, 1);

    comboBoxAuthors = new QComboBox(this);
    gridLayout->addWidget(comboBoxAuthors, 2, 1, 1, 1);
    comboBoxAuthors->setEditable(true);
    comboBoxAuthors->addItems(mparent->mAuthors);


    label_6 = new QLabel(tr("Group :"),this);
    gridLayout->addWidget(label_6, 3, 0, 1, 1);

    comboBoxGroups = new QComboBox(this);
    gridLayout->addWidget(comboBoxGroups, 3, 1, 1, 1);
    comboBoxGroups->setEditable(true);
    comboBoxGroups->addItems(mparent->mGroups);

    label_4 = new QLabel(tr("Properties :"),this);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 4, 0, 1, 1);

    textEditProperties = new QTextEdit(this);

    gridLayout->addWidget(textEditProperties, 4, 1, 1, 1);
    registerField("Title*", lineEditTitle);
    registerField("Comment", lineEditComment);
    registerField("Properties", textEditProperties);


}
void OutputFilesPage::initializePage()
{
    if(mFile.isEmpty())
     mFile = field("filePath").toString();

    BookInfo *book=new BookInfo;
    if(!book->load(mFile)){
        delete book;
        return;
    }
    QPixmap pix(book->iconPath());
    toolButtonImage->setPixmap(pix);
    mparent->setPixmapName(book->icon());

    textEditProperties->setText(book->properties());
    lineEditTitle->setText(book->title());
    comboBoxAuthors->insertItem(0,book->author());
    comboBoxAuthors->setCurrentIndex(0);

    delete book;

    /*
    QString path = field("filePath").toString();
    QFileInfo  fi(path);
    if(fi.exists()){

        Poppler::Document   *doc = Poppler::Document::load(path);
        if (doc) {
            if(doc->isLocked())
            {
                QString password = QInputDialog::getText(this, tr("Unlock %1").arg(QFileInfo(path).completeBaseName()), tr("Password:"), QLineEdit::Password);

                if(doc->unlock(password.toLatin1(), password.toLatin1()))
                {
                    delete doc;
                    return;
                }
            }

            if(doc->numPages()<1)return;


                Poppler::Page* page= doc->page(0);

                QSizeF pageSize = page->pageSizeF();//150

                qreal w=physicalDpiX() / 72.0 * pageSize.width();
                qreal h=physicalDpiX() / 72.0 * pageSize.height();

                qreal scaleFactor = qMin( 256.0/w ,   256.0/h);

                QImage  image = page->renderToImage(scaleFactor * physicalDpiX(), scaleFactor * physicalDpiY());

                QPixmap pix=QPixmap::fromImage(image);
                toolButtonImage->setPixmap(pix);
                mparent->setPixmap(pix);

                QStringList tlist= doc->infoKeys();
                QString info;
                foreach (QString s, tlist) {
                    QString value=doc->info(s);
                    if(s=="Author"){
                        comboBoxAuthors->insertItem(0,value);
                        comboBoxAuthors->setCurrentIndex(0);

                    }

                    if(value.startsWith("D:"))
                    {
                        value = doc->date(s).toString();
                    }


                    info+=s+" :"+value+"\n";

                }

                info+=tr("Num Pages :")+QString::number(doc->numPages());

                textEditProperties->setText(info);
                lineEditTitle->setText(fi.baseName());



        }
    }

    */

}
