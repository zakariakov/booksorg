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
#include "addfolderwizard.h"
#include "ui_addfolderwizard.h"

#include <QFileDialog>
#include <QTextCodec>
#include <QPushButton>

#include <QMessageBox>
//#include <QDebug>
#include <QTimer>

//#define thumbnails QDir::homePath()+"/."+QApplication::applicationName()+"/thumbnails"

AddFolderWizard::AddFolderWizard(const QStringList &groups,const QStringList &autors,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFolderWizard),mStop(false)
{
    ui->setupUi(this);
    ui->toolButtonOpen->setIcon(QIcon::fromTheme("folder-open",QIcon(":/icons/document-open")));

//  setAttribute(Qt::WA_DeleteOnClose);
    mbtnNext=new QPushButton(tr("Next"));

    if(isLeftToRight())
        mbtnNext->setIcon( QIcon::fromTheme("go-next",QIcon(":/icons/go-next")));
    else
    mbtnNext->setIcon(QIcon::fromTheme("go-previous", QIcon(":icons/go-previous")));

    connect(mbtnNext,SIGNAL(clicked()),this,SLOT(nextPage()));
    ui->buttonBox->addButton(mbtnNext,QDialogButtonBox::ActionRole);
     mbtnNext->setEnabled(false);
    connect(ui->lineEditFolder,SIGNAL(textChanged(QString)),
            this,SLOT(slotTextChanged(QString)));

    connect(ui->toolButtonOpen,SIGNAL(clicked()),
            this,SLOT(openFolder()));

    ui->comboBoxGroup->addItems(groups);
    ui->comboBoxAuthors->addItems(autors);
    //===========================================

}
void AddFolderWizard::closeEvent(QCloseEvent *e)
{
     mStop=true;
     e->accept();
}

AddFolderWizard::~AddFolderWizard()
{
   mStop=true;
  qDeleteAll(mListBook);
    delete ui;
}
void AddFolderWizard::nextPage()
{

    ui->stackedWidget->setCurrentIndex(1);
    mbtnNext->setEnabled(false);

    QTimer::singleShot(10,this,SLOT(readPdfs()));

}

void AddFolderWizard::openFolder()
{
    QString dir = QFileDialog::getExistingDirectory(0, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    ui->lineEditFolder->setText(dir);

}

void AddFolderWizard::slotTextChanged(const QString &arg)
{
    QFileInfo fi(arg);
    if(fi.isDir())
        mbtnNext->setEnabled(true);
    else
        mbtnNext->setEnabled(false);

}

void AddFolderWizard::readPdfs()
{


    QString path = ui->lineEditFolder->text();
    QDir dir(path);
    QStringList list=dir.entryList(QStringList()<<"*.pdf",QDir::Files);
    if(list.count()<1){
        QMessageBox::information(this,"",tr("this folder no contains pdf files"));
        ui->stackedWidget->setCurrentIndex(0);
        ui->lineEditFolder->setText(QString());
        return;
    }
    ui->progressBar->setRange(0,list.count());

    int i=0;
    foreach (QString file, list) {
        ui->labelProgressInfo->setText(file);

        i++;
        ui->progressBar->setValue(i);

        qApp->processEvents();
        if(mStop)return;
        qApp->processEvents();
        BookInfo *book=new BookInfo;
        if(!book->load(path+"/"+file)){
            delete book;
            continue;
        }
        if(!ui->checkBoxAthors->isChecked())
            book->setAuthor(ui->comboBoxAuthors->currentText());

        book->setGroup(ui->comboBoxGroup->currentText());
        mListBook.append(book);
    }

    ui->labelProgressInfo->setText(tr("Finished"));
    QMessageBox::information(this,"Finish",tr("Click cancel button to close this wizard"));
    this->accept();
}
void AddFolderWizard::on_buttonBox_rejected()
{
    mStop=true;
}
