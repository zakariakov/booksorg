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

    enum MyIcon{
        OPEN,
        REMOVE,
        ADD,
        EDIT,
        VIEW_ICONS,
        VIEW_DETAILS,
        VIEW_TREE,
        WIZARD,
        VIEW_CLOSE,
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
        CONFIGURE


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

        case VIEW_TREE:
            return QKeySequence();

        case GO_HOME:
            return QKeySequence("Home");

        case WIZARD:
            return QKeySequence("Ctrl+W");

        case VIEW_CLOSE:
            return QKeySequence();

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
            return QIcon::fromTheme("document-open",QIcon(":/icons/document-open"));

        case REMOVE:
            return QIcon::fromTheme("list-remove",QIcon(":/icons/list-remove"));

        case ADD:
            return QIcon::fromTheme("list-add",QIcon(":/icons/list-add"));

        case EDIT:
            return QIcon::fromTheme("document-edit",QIcon(":/icons/document-edit"));

        case VIEW_ICONS:
            return QIcon::fromTheme("view-list-icons",QIcon(":/icons/view-list-icons"));

        case VIEW_DETAILS:
            return QIcon::fromTheme("view-list-details",QIcon(":/icons/view-list-details"));

        case VIEW_TREE:
            return QIcon::fromTheme("view-list-tree",QIcon(":/icons/view-list-tree"));

        case GO_HOME:
            return QIcon::fromTheme("user-home",QIcon(":/icons/go-home"));

        case WIZARD:
            return QIcon::fromTheme("tools-wizard",QIcon(":/icons/tools-wizard"));

        case VIEW_CLOSE:
            if(QApplication::isLeftToRight())
                return QIcon::fromTheme("view-left-close",QIcon(":/icons/view-left-close"));
            else
                return QIcon::fromTheme("view-right-close",QIcon(":/icons/view-right-new"));

        case GO_FIRST:
            if(QApplication::isLeftToRight())
                return QIcon::fromTheme("go-first", QIcon(":icons/go-first"));
            else
                return QIcon::fromTheme("go-last",QIcon(":/icons/go-last"));

        case GO_PREVIOUS:
            if(QApplication::isLeftToRight())
                return QIcon::fromTheme("go-previous", QIcon(":icons/go-previous"));
            else
                return QIcon::fromTheme("go-next",QIcon(":/icons/go-next"));

        case GO_NEXT:
            if(QApplication::isLeftToRight())
                return QIcon::fromTheme("go-next", QIcon(":icons/go-next"));
            else
                return QIcon::fromTheme("go-previous", QIcon(":icons/go-previous"));

        case GO_LAST:
            if(QApplication::isLeftToRight())
                return QIcon::fromTheme("go-last",QIcon(":/icons/go-last"));
            else
                return QIcon::fromTheme("go-first", QIcon(":icons/go-first"));

        case ZOOM_BEST:
            return QIcon::fromTheme("zoom-fit-best",QIcon(":/icons/zoom-fit-best"));

        case ZOOM_FIT:
            return QIcon::fromTheme("zoom-fit-width",QIcon(":/icons/zoom-fit-width"));

        case ZOOM_ORIG:
            return QIcon::fromTheme("zoom-original",QIcon(":/icons/zoom-original"));

        case ZOOM_IN:
            return QIcon::fromTheme("zoom-in",QIcon(":/icons/zoom-in"));

        case ZOOM_OUT:
            return QIcon::fromTheme("zoom-out",QIcon(":/icons/zoom-out"));

        case GROOPS:
            return QIcon(":/icons/group32");

        case AUTORS:
            return QIcon(":/icons/author32");

        case FAVO:
            return QIcon(":/icons/favo32");

        case VIEW_LIST:
            return QIcon::fromTheme("view-list-text",QIcon(":/icons/view-list-text"));

        case QUITE:
            return QIcon::fromTheme("application-exit",QIcon(":/icons/application-exit"));

        case FIND:
            return QIcon::fromTheme("find",QIcon(":/icons/find"));

        case UNDO:
            return QIcon::fromTheme("undo",QIcon(":/icons/undo"));

        case RELOAD:
            return QIcon::fromTheme("reload",QIcon(":/icons/reload"));

        case EDIT_CLEAR:
            return QIcon::fromTheme("edit-clear",QIcon(":/icons/edit-clear"));

        case BOOKMARK:
            return QIcon::fromTheme("bookmark",QIcon(":/icons/bookmark"));

        case BOOKMARK_ADD:
            return QIcon::fromTheme("bookmark_add",QIcon(":/icons/bookmark_add"));

        case RECENT:
            return QIcon::fromTheme("clock",QIcon(":/icons/recent"));
        case INFO_HELP:
            return QIcon::fromTheme("help-about",QIcon(":/icons/help-about"));

        case BOOKS_TAB:
            return QIcon(":/icons/tabbooks");
        case MENU_LIST:
            return QIcon(":/icons/menu-list");

        case FULL_SCREEN:
            return QIcon::fromTheme("view-fullscreen", QIcon(":/icons/view-fullscreen"));

        case CONFIGURE:
            return QIcon::fromTheme("document-properties", QIcon(":/icons/document-properties"));

        default:
            break;
        }
        return QIcon::fromTheme("unknown");
//#endif
    }


};
#endif // EICON_H
