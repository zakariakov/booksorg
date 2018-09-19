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


#ifndef SEARCHTOOLS_H
#define SEARCHTOOLS_H

#include <QWidget>
#include <QIcon>
#include <QtCore>
#include <QCompleter>

#include <QtGui>
namespace Ui {
class SearchTools;
}



class SearchTools : public QWidget
{
    Q_OBJECT
    
public:
    explicit SearchTools(bool isPdf=true,QWidget *parent = nullptr);
    ~SearchTools();
    void show();
signals:

    void searchPressed(const QString &);
    void searchCleared();
    void searchPreviuos();
    void searchNext();

private slots:

    void on_toolButtonClose_clicked();

    void on_lineEditFind_returnPressed();

    void on_toolButtonClear_clicked();

    void on_toolButtonPrev_clicked();

    void on_toolButtonNext_clicked();

    void on_toolButtonFind_clicked();

private:
    Ui::SearchTools *ui;
   QAction *actMatchCase;
 QCompleter *mCompleter;
 QStringList mList;
public slots:
     bool isMachCase();
     Qt::LayoutDirection inputDirection() const;

};

#endif // SEARCHTOOLS_H
