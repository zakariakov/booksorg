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

#ifndef BOOKINFO_H
#define BOOKINFO_H
#include <QString>
#if QT_VERSION >= 0x050000
#include <poppler-qt5.h>
#else
#include <poppler-qt4.h>
#endif
#include<QPaintDevice>
#include<QApplication>
#include<QInputDialog>
#include<QDir>
#include<QFileInfo>
#include<QObject>
class BookInfo :public QObject
{
    Q_OBJECT

public:
    BookInfo();
   bool load(const QString &path);

    const QString &author() const { return mAuthor; }
    const QString &title() const { return mTitle; }
    const QString &comment() const { return mComment; }
    const QString &path() const { return mPath; }
    const QString &properties() const { return mProperties; }
    const QString &icon() const{ return mIcon; }
    const QString iconPath() { return mThumbnails+mIcon; }
    const QString &group() const{ return mGroup; }

    void setAuthor(const QString &aut){mAuthor=aut;}
    void setTitle(const QString &title){mTitle=title;}
    void setComment(const QString &description){mComment=description;}
    void setIcon(const QString &icon){mIcon=icon;}
    void setPath(const QString &path){mPath=path;}
    void setProperties(const QString &p){mProperties=p;}
    void setGroup(const QString &g){mGroup=g;}

protected:
    QString mAuthor;
    QString mTitle;
    QString mComment;
    QString mIcon;
    QString mPath;
    QString mProperties;
    QString mGroup;
private:
    QString mThumbnails;

};

#endif // BOOKINFO_H
