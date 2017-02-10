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

#ifndef ADDFOLDERWIZARD_H
#define ADDFOLDERWIZARD_H
#include "bookinfo.h"

#include <QDialog>
#include <QCloseEvent>
#include <QFileInfo>
namespace Ui {
class AddFolderWizard;
}

class AddFolderWizard : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddFolderWizard(const QStringList &groups,const QStringList &autors, QWidget *parent = 0);
    ~AddFolderWizard();
    void closeEvent(QCloseEvent *e);
    QList<BookInfo *> getListBook(){return mListBook;}
signals:

private:
    Ui::AddFolderWizard *ui;

    QList<BookInfo *>mListBook;
bool mStop;
QPushButton *mbtnNext;
private slots:
    void openFolder();
    void nextPage();
    void slotTextChanged(const QString &arg);
    void readPdfs();
    //void addBook(QFileInfo fi,Poppler::Document   *doc);
    void on_buttonBox_rejected();
};

#endif // ADDFOLDERWIZARD_H
