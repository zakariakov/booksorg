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

#ifndef EDITBOOKDIALOG_H
#define EDITBOOKDIALOG_H
#include "ratingwidget.h"
#include <QDialog>
#include <QCompleter>
#include <QStandardItemModel>
namespace Ui {
class EditBookDialog;
}

class EditBookDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditBookDialog(const bool &isRoot,const bool &info=false, QWidget *parent = 0);
    ~EditBookDialog();

    QString path();
    void setPath(const QString &arg);

    QString title();
    void setTitle(const QString &arg);

    QString comment();
    void setComment(const QString &arg);

    QString author();
    void setAuthor(const QStringList &list, const QString &arg);

    QString group();
    void setGroup(const QStringList &list, const QString &arg);

    int rating();
    void setRating(const int &arg);

    QString tags();
    void setTags(const QString &arg);

    void setIcon(const QIcon &arg);

    void setPropertiesModel(QStandardItemModel *m);
    void setFontsModel(QStandardItemModel *m);
signals:
   void getFontModel(EditBookDialog *w);
private:
    Ui::EditBookDialog *ui;
    RatingWidget *mRatingWidget;
    QString mPath;
    int mRating;
    QStringList   mList;
    QCompleter  * mCompleter ;

private slots:
    void accept();
    //    void setNewRating();

    //   void on_lineEditTag_textChanged(const QString &arg1);
    void on_tabWidget_currentChanged(int index);
};

#endif // EDITBOOKDIALOG_H
