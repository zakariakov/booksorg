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
#include "bookinfo.h"
#if QT_VERSION >= 0x050000
#include <poppler-qt5.h>
#else
#include <poppler-qt4.h>
#endif

#include<QApplication>
#include<QInputDialog>
#include<QDir>
#include<QFileInfo>
#include<QSettings>
//#define thumbnails QDir::homePath()+"/."+QApplication::applicationName()+"/thumbnails"

BookInfo::BookInfo()
{

QSettings settings;

QString appdir=(settings.value("BooksPath",
                                             QDir::homePath()+"/."+QApplication::applicationName()).toString());
mThumbnails=appdir+"/thumbnails/";

}

bool BookInfo::load(const QString &path)
{
    QFileInfo  fi(path);

    if(!fi.exists())return false;

    Poppler::Document   *doc = Poppler::Document::load(path);

    if (!doc) return false;

    if(doc->isLocked())
    {
        QString password = QInputDialog::getText(0, QObject::tr("Unlock %1").arg(QFileInfo(path).completeBaseName()), tr("Password:"), QLineEdit::Password);

        if(doc->unlock(password.toLatin1(), password.toLatin1()))
        {
            delete doc;
            return false;
        }
    }

    if(doc->numPages()<1){
        delete doc;
        return false;
    }

    //BEGIN PIXMAP---------------------------------------------------

    Poppler::Page* page= doc->page(0);

    QSizeF pageSize = page->pageSizeF();
    QPixmap pix(256,256);
    qreal w=pix.physicalDpiX() / 72.0 * pageSize.width();
    qreal     h=pix.physicalDpiX() / 72.0 * pageSize.height();

    qreal scaleFactor = qMin( 256.0/w ,   256.0/h);

    QImage  image = page->renderToImage(scaleFactor * pix.physicalDpiX(), scaleFactor * pix.physicalDpiY());

    pix=QPixmap::fromImage(image);

    QString pixName=fi.baseName();
    pixName.replace(" ","_");
    if(QFile::exists(mThumbnails+pixName+".png")){
        QString n=pixName;
        int i=0;
        do{
            i++;
            pixName=n+QString::number(i);
        }while (QFile::exists(mThumbnails+pixName+".png"));
    }

    pix.save(mThumbnails+pixName+".png");

    //END PIXMAP---------------------------------------------------


    //BEGIN INFO---------------------------------------------------
    QStringList tlist= doc->infoKeys();

    foreach (QString s, tlist) {
        QString value=doc->info(s);

        if(s=="Author"){
            mAuthor=value;
            if(!value.isEmpty())
            mProperties+=s+" :"+value+"\n";
        }

        if(value.startsWith("D:"))
        {
            value = doc->date(s).toString();
        }

          if(s=="CreationDate"||s=="Title"||s=="Creator"
                  ||s=="Producer"||s=="ModDate"||s=="Format")
              if(!value.isEmpty())
        mProperties+=s+" :"+value+"\n";

    }

    mProperties+=("Pages :")+QString::number(doc->numPages());

    //END INFO---------------------------------------------------

    mTitle=fi.completeBaseName();
    mIcon= pixName+".png";
    mPath=path;

    delete doc;
return true;
}
