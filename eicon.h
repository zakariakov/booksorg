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

#ifndef EICON_H
#define EICON_H
#include <QString>
#include <QIcon>
#include <QApplication>
#include <QSettings>
#include <QKeySequence>
#include <QStyle>
#include <QDir>
#include <QPainter>
#include <QDebug>
    enum MyIcon{
        OPEN,
        REMOVE,
        ADD,
        EDIT,
        VIEW_ICONS,
        VIEW_DETAILS,
        WIZARD,
        GO_HOME,
        GO_FIRST,
        GO_PREVIOUS,
        GO_NEXT,
        GO_LAST,
        ZOOM_IN,
        ZOOM_OUT,
        ZOOM_FIT,
        ZOOM_BEST,
        ZOOM_ORIG,
        GROOPS,
        AUTORS,
        FAVO,
        VIEW_LIST,
        QUITE,
        FIND,
        UNDO,
        RELOAD,
        EDIT_CLEAR,
        BOOKMARK,
        BOOKMARK_ADD,
        RECENT,
        BOOKS_TAB,
        MENU_LIST,
        INFO_HELP,
        FULL_SCREEN,
        CONFIGURE,
        APP_PDF,
        RENAME


    };
class EIcon
{
public:

    static  QString themeName()
    {


        QSettings setting("elokab","elokabsettings");
        setting.beginGroup("Themes");
        QString icnThem=  setting.value("iconsTheme",QIcon::themeName()).toString();
        setting.endGroup();

        if(icnThem=="hicolor"||icnThem.isEmpty()){

            QStringList failback;
            failback << "oxygen"<< "Mint-X"
                     << "Humanity"<< "Tango"
                     << "Prudence-icon"<< "elementary"<< "gnome";

            QDir dir("/usr/share/icons/");
            foreach (QString s, failback)
            {
                if (dir.exists(s))
                {
                    icnThem=s;

                    break;
                }
            }
        }

        if(icnThem.isEmpty())
            icnThem="hicolor";

        //QIcon::setThemeName(icnThem);

        return icnThem;
    }

    static  QKeySequence shortcut( MyIcon name )
    {
        switch (name) {
        case OPEN:
            return QKeySequence();

        case REMOVE:
            return QKeySequence::Delete;

        case ADD:
            return QKeySequence::Open;

        case EDIT:
            return QKeySequence("Ctrl+E");

        case VIEW_ICONS:
            return QKeySequence("Ctrl+1");

        case VIEW_DETAILS:
            return QKeySequence("Ctrl+2");



        case GO_HOME:
            return QKeySequence("Home");

        case WIZARD:
            return QKeySequence("Ctrl+W");



        case GO_FIRST:
            return QKeySequence::MoveToStartOfDocument;

        case GO_PREVIOUS:
            return QKeySequence::MoveToPreviousPage;

        case GO_NEXT:
            return QKeySequence::MoveToNextPage;

        case GO_LAST:
            return QKeySequence::MoveToEndOfDocument;

        case ZOOM_BEST:
            return QKeySequence("Ctrl+B");

        case ZOOM_FIT:
            return QKeySequence("Ctrl+H");

        case ZOOM_ORIG:
            return QKeySequence("Ctrl+=");

        case ZOOM_IN:
            return QKeySequence::ZoomIn;

        case ZOOM_OUT:
            return QKeySequence::ZoomOut;

        case GROOPS:
            return QKeySequence("Ctrl+G");

        case AUTORS:
            return QKeySequence("Ctrl+T");

        case FAVO:
            return QKeySequence("Ctrl+N");

        case VIEW_LIST:
            return QKeySequence();

        case QUITE:
            return QKeySequence::Quit;

        case FIND:
            return QKeySequence::Find;

        case UNDO:
            return QKeySequence::Undo;

        case RELOAD:
            return QKeySequence::Refresh;

        case EDIT_CLEAR:
            return QKeySequence();

        case  BOOKMARK:
            return QKeySequence();

        case BOOKMARK_ADD:
            return QKeySequence();

        case RECENT:
            return QKeySequence();

        case BOOKS_TAB:
            return QKeySequence();

        case MENU_LIST:
            return QKeySequence();

        case INFO_HELP:
            return QKeySequence();

        case FULL_SCREEN:
            return QKeySequence("F11");

        case CONFIGURE:
            return QKeySequence();


        default:
            break;
        }
           return QKeySequence();

    }

    static  QIcon icon( MyIcon name )
    {


        switch (name) {

        case OPEN:
            //return QIcon::fromTheme("document-open",QIcon(":/icons/document-open"));
            return charIcon("");//

        case REMOVE:
            //return QIcon::fromTheme("list-remove",QIcon(":/icons/list-remove"));
            return charIcon("\uF056");//

        case ADD:
            //return QIcon::fromTheme("list-add",QIcon(":/icons/list-add"));
            return charIcon("\uF055"); // 

        case EDIT:
            //return QIcon::fromTheme("document-edit",QIcon(":/icons/document-edit"));
            return charIcon("\uF040");//

        case VIEW_ICONS:
            //return QIcon::fromTheme("view-list-icons",QIcon(":/icons/view-list-icons"));
            return charIcon("\uF009");//

        case VIEW_DETAILS:
            //return QIcon::fromTheme("view-list-details",QIcon(":/icons/view-list-details"));
            return charIcon("\uF00B");//


        case GO_HOME:
            //return QIcon::fromTheme("user-home",QIcon(":/icons/go-home"));
            return charIcon("\uF015");//

        case WIZARD:
            //return QIcon::fromTheme("tools-wizard",QIcon(":/icons/tools-wizard"));
            return charIcon("\uF0D0");//



        case GO_FIRST:
            if(QApplication::isLeftToRight())
                //return QIcon::fromTheme("go-first", QIcon(":icons/go-first"));
                return charIcon("\uF100");// 
            else
                // return QIcon::fromTheme("go-last",QIcon(":/icons/go-last"));
                return charIcon("\uF101");//

        case GO_PREVIOUS:
            if(QApplication::isLeftToRight())
                //return QIcon::fromTheme("go-previous", QIcon(":icons/go-previous"));
                return charIcon("\uF104");// 
            else
                //return QIcon::fromTheme("go-next",QIcon(":/icons/go-next"));
                return charIcon("\uF105");//

        case GO_NEXT:
            if(QApplication::isLeftToRight())
                //return QIcon::fromTheme("go-next", QIcon(":icons/go-next"));
                return charIcon("\uF105"); // 
            else
                //return QIcon::fromTheme("go-previous", QIcon(":icons/go-previous"));
                return charIcon("\uF104"); // 

        case GO_LAST:
            if(QApplication::isLeftToRight())
                //return QIcon::fromTheme("go-last",QIcon(":/icons/go-last"));
                return charIcon("\uF101");// 
            else
                //return QIcon::fromTheme("go-first", QIcon(":icons/go-first"));
                return charIcon("\uF100");// 

        case ZOOM_BEST:
            //return QIcon::fromTheme("zoom-fit-best",QIcon(":/icons/zoom-fit-best"));
            return charIcon("\uF07D");// 

        case ZOOM_FIT:
            //return QIcon::fromTheme("zoom-fit-width",QIcon(":/icons/zoom-fit-width"));
            return charIcon("\uF07E");// 

        case ZOOM_ORIG:
            //return QIcon::fromTheme("zoom-original",QIcon(":/icons/zoom-original"));
            return charIcon("\uF066");// 

        case ZOOM_IN:
            //return QIcon::fromTheme("zoom-in",QIcon(":/icons/zoom-in"));
            return charIcon("\uF00E");// 

        case ZOOM_OUT:
            //return QIcon::fromTheme("zoom-out",QIcon(":/icons/zoom-out"));
            return charIcon("\uF010");// 

        case GROOPS:
            // return QIcon(":/icons/group32");
            return charIcon("\uF029");// 

        case AUTORS:
            // return QIcon(":/icons/author32");
            return charIcon("\uF007");// 

        case FAVO:
            //return QIcon(":/icons/favo32");
            return charIcon("\uF005");// 

        case VIEW_LIST:
            //return QIcon::fromTheme("view-list-text",QIcon(":/icons/view-list-text"));
            return charIcon("\uF039");// 

        case QUITE:
            //return QIcon::fromTheme("application-exit",QIcon(":/icons/application-exit"));
            return charIcon("\uF057"); // 

        case FIND:
            //return QIcon::fromTheme("find",QIcon(":/icons/find"));
            return charIcon("\uF05B");//

        case UNDO:
            //return QIcon::fromTheme("undo",QIcon(":/icons/undo"));
            return charIcon("\uF064");//

        case RELOAD:
            //return QIcon::fromTheme("reload",QIcon(":/icons/reload"));
            return charIcon("\uF01E");//

        case EDIT_CLEAR:
            //return QIcon::fromTheme("edit-clear",QIcon(":/icons/edit-clear"));
            return charIcon("\uF2D4");//

        case BOOKMARK:
            //return QIcon::fromTheme("bookmark",QIcon(":/icons/bookmark"));
            return charIcon("\uF02E");//  \uF097

        case BOOKMARK_ADD:
            // return QIcon::fromTheme("bookmark_add",QIcon(":/icons/bookmark_add"));
            return charIcon("\uF055");// 

        case RECENT:
            //return QIcon::fromTheme("clock",QIcon(":/icons/recent"));
            return charIcon("\uF017"); // 

        case INFO_HELP:
            //return QIcon::fromTheme("help-about",QIcon(":/icons/help-about"));
            return charIcon("\uF06A"); // 

        case BOOKS_TAB:
            return QIcon(":/icons/tabbooks");

        case MENU_LIST:
            //return QIcon(":/icons/menu-list");
            return charIcon("\uF039");//

        case FULL_SCREEN:
            //return QIcon::fromTheme("view-fullscreen", QIcon(":/icons/view-fullscreen"));
            return charIcon("\uF0B2");//

        case CONFIGURE:
            // return QIcon::fromTheme("document-properties", QIcon(":/icons/document-properties"));
            return charIcon("\uF022");//

        case APP_PDF:
            // return QIcon::fromTheme("document-properties", QIcon(":/icons/document-properties"));
            return charIcon("\uF1C1");//

        case RENAME:
            // return QIcon::fromTheme("document-properties", QIcon(":/icons/document-properties"));
            return charIcon("\uF044");//

        default:
            break;
        }

        return QIcon::fromTheme("unknown");

    }

private:
  static  QIcon  charIcon(const QString &name)
    {
        QFont font("FontAwesome",16);
        QFontMetrics fm(font);
        int w= fm.width(name);
        int h= fm.xHeight();

        int wh=w;
        if(h>w)wh=h;

       //qDebug()<<"FontAwesome======:"<<name<<h<<w<<wh;

        QPixmap pix(wh+4,wh+4);
        pix.fill(Qt::transparent);
        QPainter p(&pix);

        p.setFont(font);
        p.setPen(qApp->palette().windowText().color());
        //  p.drawText(0,0,name-2);
        QRect rect(0,0,wh+4,wh+4);
        p.drawText(rect, Qt::AlignCenter,name);

        return QIcon(pix);
    }

};
#endif // EICON_H
