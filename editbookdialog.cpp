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

#include "editbookdialog.h"
#include "ui_editbookdialog.h"
#include <QSettings>
#include <QFileInfo>
#include <QApplication>
#include <QDir>
#include <QTextCodec>
//#include <QDebug>
#include <QCompleter>
#include <QStandardItemModel>
//#define thumbnails QDir::homePath()+"/."+QApplication::applicationName()+"/thumbnails"
//#define BooksPath QDir::homePath()+"/."+QApplication::applicationName()+"/books"

EditBookDialog::EditBookDialog(const bool &isRoot,const bool &info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBookDialog)
{
    ui->setupUi(this);
if(!info)
{
    ui->tabWidget->removeTab(2);
    ui->tabWidget->removeTab(1);

}
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)

    ui->tableViewFont->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewFont->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewProperties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewProperties->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

#else

    ui->tableViewFont->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewFont->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewProperties->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewProperties->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

#endif // QT_VERSION

    mRatingWidget =new RatingWidget(false);
    ui->horizontalLayout->addWidget(mRatingWidget);

    if(isRoot){
        mPath=mPath+"/.directory";
        ui->comboBoxAuthors->setVisible(false);
        ui->lineEditPath->setVisible(false);
        ui->labelAuthor->setVisible(false);
        ui->labelPath->setVisible(false);
        ui->labelStars->setVisible(false);
        mRatingWidget->
                setVisible(false);
        ui->labelGroup->setVisible(false);
        ui->comboBoxGroup->setVisible(false);
        ui->lineEditComment->setVisible(false);
        ui->labelComment->setVisible(false);
        ui->lineEditTag->setVisible(false);
    }

    QSettings setting;
    mList= setting.value("TagCompliter").toStringList();
    mCompleter = new QCompleter(mList, this);
    mCompleter->setMaxVisibleItems(7);
    mCompleter->setCompletionPrefix(",");
    mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditTag->setCompleter(mCompleter);


}

EditBookDialog::~EditBookDialog()
{
    delete ui;
}


void EditBookDialog::accept()
{

    if(!ui->lineEditTag->text().isEmpty())
    {

        QStringList list=ui->lineEditTag->text().split(",");
        foreach (QString s, list) {
            if(!mList.contains(s))
                mList.append(s);
        }

        QSettings setting;
        setting.setValue("TagCompliter",mList);

    }

    QDialog::accept();

}


QString EditBookDialog::path()
{
   return ui->lineEditPath->text();
}

QString EditBookDialog::title()
{
    return ui->lineEditTitle->text();
}

QString EditBookDialog::comment()
{
     return ui->lineEditComment->text();
}

QString EditBookDialog::author()
{
     return ui->comboBoxAuthors->currentText();
}

QString EditBookDialog::group()
{
     return ui->comboBoxGroup->currentText();
}

int EditBookDialog::rating()
{
    return mRatingWidget->rating();
}

void EditBookDialog::setPath(const QString &arg)
{
    ui->lineEditPath->setText(arg);
}

void EditBookDialog::setTitle(const QString &arg)
{
    ui->lineEditTitle->setText(arg);
}

void EditBookDialog::setComment(const QString &arg)
{
     ui->lineEditComment->setText(arg);
}

void EditBookDialog::setAuthor(const QStringList &list, const QString &arg)
{
      ui->comboBoxAuthors->addItems(list);
     ui->comboBoxAuthors->setCurrentIndex(-1);
     ui->comboBoxAuthors->setEditText(arg);

}

void EditBookDialog::setGroup(const QStringList &list, const QString &arg)
{
    ui->comboBoxGroup->addItems(list);
    ui->comboBoxGroup->setCurrentIndex(-1);
    ui->comboBoxGroup->setEditText(arg);

}
void EditBookDialog::setRating(const int &arg)
{
//qDebug()<<"EditBookDialog::setRating "<<arg;
mRatingWidget->setRating(arg);

}

QString EditBookDialog::tags()
{
    return ui->lineEditTag->text();
}

void EditBookDialog::setTags(const QString &arg)
{
    ui->lineEditTag->setText(arg);
}

void EditBookDialog::setIcon(const QIcon &arg)
{
    ui->toolButtonImage->setIcon(arg);
}

void EditBookDialog::setPropertiesModel(QStandardItemModel *m)
{
    ui->tableViewProperties->setModel(m);
}
void EditBookDialog::setFontsModel(QStandardItemModel *m)
{
    ui->tableViewFont->setModel(m);
}


//void EditBookDialog::on_lineEditTag_textChanged(const QString &arg1)
//{


//}

void EditBookDialog::on_tabWidget_currentChanged(int index)
{
    if(index==2)
        emit getFontModel(this);
}
