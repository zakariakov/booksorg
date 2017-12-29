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

#ifndef WIDGETBOOKMARK_H
#define WIDGETBOOKMARK_H

#include <QWidget>
#include <QTreeWidget>
#include <QToolBar>
#include <QAction>

class WidgetBookmark : public QWidget
{
    Q_OBJECT

public:

    //! البناء
    explicit WidgetBookmark(QWidget *parent = 0);

signals:

    //! اشارة لفتح ملف
    void openDocument(const QString,int,qreal,qreal);

    //! اشارة لجلب معلومات الصفحة الحالية
    void getBookmark();

public slots:

    //! اضافة مفضلة جديدة
    void addBookmark(const QString &path, int page, qreal left, qreal top, const QString &str=QString());

private slots:

    //! حفظ المفظلات
    void saveBookmarkSettings();

    //! تحميل المفظلات
    void loadBookmarkSettings();

    //! تفعيل عنصر المفظلة
    void treeWidgetBookmarkItemActivated(QTreeWidgetItem *item, int);

    //!  حذف المفظلة المحددة
    void removeBookmark();

    //! اعادة تسمية المحددة
    void renameBookmark();

    //! القائمة المنسدلة
    void customMenuBookmark(QPoint);

private:

    QTreeWidget *treeWidgetBookmark;
    QToolBar    *mToolBarBookmark;
    QAction     *actAddBookmark;


};

#endif // WIDGETBOOKMARK_H
