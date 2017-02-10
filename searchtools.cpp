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

#include "searchtools.h"
#include "ui_searchtools.h"
#include "eicon.h"
#include <QtGui>
#include <QSettings>

SearchTools::SearchTools(bool isPdf, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchTools)
{
    ui->setupUi(this);

    ui->toolButtonClose->setIcon(EIcon::icon(QUITE));
    ui->toolButtonClear->setIcon(EIcon::icon(EDIT_CLEAR));
    ui->toolButtonFind->setIcon(EIcon::icon(FIND));

    actMatchCase=new QAction(tr("Match case"),this);
    actMatchCase->setCheckable(true);
    if(isPdf)
        actMatchCase->setChecked(true);
    else
        actMatchCase->setChecked(false);

    ui->toolButtonFind->addAction(actMatchCase);

    ui->toolButtonNext->setShortcut(QKeySequence::FindNext);
    ui->toolButtonPrev->setShortcut(QKeySequence::FindPrevious);

    ui->toolButtonNext->setToolTip(tr("Find Next \n")+ ui->toolButtonNext->shortcut().toString());
    ui->toolButtonPrev->setToolTip(tr("Find Previous \n")+ ui->toolButtonPrev->shortcut().toString());


    if(!isPdf){
        ui->toolButtonNext->setVisible(false);
        ui->toolButtonPrev->setVisible(false);

    }
 ui->toolButtonPrev->setVisible(false);
    QSettings setting;
   mList= setting.value("Compliter").toStringList();
    mCompleter = new QCompleter(mList, this);
    mCompleter->setMaxVisibleItems(7);
    ui->lineEditFind->setCompleter(mCompleter);
}

SearchTools::~SearchTools()
{
    delete ui;
}

//____________________________________________
void SearchTools::show()
{
    ui->lineEditFind->setFocus();
}

//____________________________________________
void SearchTools::on_toolButtonClose_clicked()
{
    ui->lineEditFind->clear();
    hide();
    emit searchCleared();
}

//_______________________________________________
void SearchTools::on_lineEditFind_returnPressed()
{

    if(!ui->lineEditFind->text().isEmpty()){
        if(!mList.contains(ui->lineEditFind->text())){
            mList.append(ui->lineEditFind->text());

            if(mList.count()>100)mList.removeFirst();

            QSettings setting;
            setting.setValue("Compliter",mList);
          }
        emit searchPressed(ui->lineEditFind->text());
    }
}
//______________________________________________
void SearchTools::on_toolButtonFind_clicked()
{

    on_lineEditFind_returnPressed();
}
//____________________________________________
void SearchTools::on_toolButtonClear_clicked()
{
    ui->lineEditFind->clear();
    emit searchCleared();
}
//____________________________________________
void SearchTools::on_toolButtonPrev_clicked()
{
    if(!ui->lineEditFind->text().isEmpty())
    emit searchPreviuos();
}
//____________________________________________
void SearchTools::on_toolButtonNext_clicked()
{

     if(!ui->lineEditFind->text().isEmpty())
     emit searchNext();
}
//____________________________________________
 bool SearchTools::isMachCase()
 {
     return actMatchCase->isChecked();

 }


Qt::LayoutDirection SearchTools::inputDirection() const
{

  qDebug()<<"SearchTools::inputDirection() layoutDirection:"<<ui->lineEditFind->layoutDirection();

   return ui->lineEditFind->layoutDirection();
  }
