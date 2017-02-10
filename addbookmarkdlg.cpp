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

#include "addbookmarkdlg.h"
#include "ui_addbookmarkdlg.h"

AddBookmarkDlg::AddBookmarkDlg(const QStringList &list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookmarkDlg)
{
    ui->setupUi(this);
      ui->comboBoxGroup->addItems(list);
}

AddBookmarkDlg::~AddBookmarkDlg()
{
    delete ui;
}

QString AddBookmarkDlg::getGroup()
{
    return ui->comboBoxGroup->currentText();
}

QString AddBookmarkDlg::getName()
{
    return ui->lineEditName->text();
}

void  AddBookmarkDlg::setBookmarkName(const QString &str)
{
  ui->lineEditName->setText(str);
}
