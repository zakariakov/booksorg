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

#ifndef RATINGWIDGET_H
#define RATINGWIDGET_H
#include <QWidget>

#include <QDialogButtonBox>

class RatingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RatingWidget(bool saveDlg=true,QWidget *parent = 0);

int rating(){return mRating;}
void setRating(int r,QString file=QString());
void clearRating();
signals:
void ratingChanged(int,QString);

public slots:

private slots:
void newRating();
void setRatingImage(int r);
void buttonBoxClicked(QAbstractButton* button);
private:
    QWidget *mWidgetStars;
int mRating;
int oldRating;
QString mFile;
bool mShowSave;
 QDialogButtonBox*   mButtonBox ;
};

#endif // RATINGWIDGET_H
