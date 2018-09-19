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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "editbookdialog.h"
#include "mymodel.h"
#include "actions.h"
#include "searchtools.h"
#include "widgetpdf.h"
#include "ratingwidget.h"

#include "widgetbookmark.h"
#include <QMainWindow>
#include <QActionGroup>
#include <QToolBar>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
void closeEvent(QCloseEvent *event);
signals:

    void   curPageChanged(int);

private slots:
void updateItem(QModelIndex index);

    //! \brief slotOpen فتح الكتاب المحدد عند النقر على الكتاب
    //!
    void slotOpen();

    //! \brief slotOpenFileName استدعاء فتح الكتاب
    //! \param fileName مسار الكتاب
    //!
    void slotOpenFileName(const QString &fileName);

    //! \brief bookTitle جلب عنوان الكتاب اذا كان موجودا
    //!  او اضافته اذا لم يكن موجودا
    //! \param fileName مسار الكتاب
    //! \param w الودجات الحاوية للكتاب
    //! \return  عنوان الكتاب
    //!
    QString bookTitle(const QString &fileName);

    //! \brief slotOpenWith فتح الكتاب بببرنامج خارجي
    //!
    void slotOpenWith();

    //! \brief addBook اضافة كتاب جديد
    //!
    bool addBook(const QString &file=QString());

    //! \brief setCurentTab في حال تغير اللسان
    //! سيتم تغيير القوائم والاوامر حسب اللسان وما يحويه
    //! \param arg معرف اللسان
    //!
    void setCurentTab(int arg);

    //! \brief scanFolder مسح مجلد لاضافة كتب
    //!
    void scanFolder();

    //! \brief editBookOrGroup تحرير المجموعة او الكتاب
    //!
    void editBookOrGroup();

    //! \brief editBookInfo تحرير الكتاب المفتوح
    //!
    void editBookInfo();

    //! \brief deletSelectedIndexes حذف العناصر المحددة
    //!
    void deletSelectedIndexes();

    //! \brief changeAuthor تغيير تسمية المؤلف
    //!
    void changeAuthor();

    //! \brief changeGroup تغيير تسمية المجموعة
    //!
    void changeGroup();

    //! \brief moveItems نقل العنصر الى مجموعة او مؤلف موجود
    //! \param authors  مجموعة او مؤلف
    //!
    void moveItems(bool authors);

    //! \brief goRootPath الانتقال الى البيت سواء مجموعة او مؤلف او تنقيط
    //!
    void goRootPath();

    //! \brief slotItemActivated عند تفعيل عنصر
    //! \param index المعرف
    //!
    void slotItemActivated(const QModelIndex & index);

    //! \brief slotcurrentRowChanged عند تحديد عنصر
    //!  يتم جلد معلومات العنصر لعرضها على نافذة المعلومات
    //! \param current معرف العنصر
    //!
    void slotcurrentRowChanged(const QModelIndex &current, const QModelIndex &);

    //! \brief customContextMenu تكوين القائمة الفرعية
    //!
    void customContextMenu(QPoint);

    //! \brief setupTools تكوين الاوامر والقوائم
    //!
    void setupTools();

    //! \brief switchSkins التغيير بين السمة التقليدية والجديدة
    //!
    void  switchSkins();

    //! \brief toggleView تغيير العرض بين الايقونات والتفاصيل
    //! \param action الامر
    //!
    void toggleView(QAction* action);

    //! \brief toggleRootPath تغيير المنزل بين مجموعات او مؤلفيين او تنقيط
    //! \param action الامر
    //!
    void toggleRootPath(QAction* action);

    //! \brief pathChanged مغرفة هل الودجات المفعلة هي لعض الايقومات او التفاصيل
    //!
    void pathChanged(QString);

    //! \brief currentIndex معرفة مغرف العنصر المفعل
    //! \return المعرف
    //!
    QModelIndex currentIndex();

    //! \brief goSearch بدأ البحث
    //! \param arg نص البحث
    //!
    void goSearch(const QString &arg);

    //! \brief updateRecentFileActions تحميل الملفات السابقة
    //!
    void updateRecentFileActions();

    //! \brief openRecentFile فتح ملف سابق
    //!
    void openRecentFile();

    //! \brief slotAbout حول البرنامج
    //!
    void slotAbout();

    //! \brief swichLanguage تغيير اللغة
    //! \param act الامر
    //!
    void swichLanguage(QAction* act);

    //! \brief switchToolBarMode امر تغيير السمة
    //! \param arg التقليدية او الجديدة
    //!
    void switchToolBarMode(bool arg);

    //! \brief slotAddBookmark اضافة مفضلة
    //!
    void slotAddBookmark();

    //! \brief bookmarkOpenDocument فتح الملف من المفضلة
    //! \param fileName الملف
    //! \param page الصفحة
    //! \param left ليسار
    //! \param top فوق
    //!
    void bookmarkOpenDocument(const QString &fileName,int page,qreal left,qreal top);

    //! \brief on_tabWidget_tabCloseRequested عند غلق اللسان
    //! \param index المعرف
    //!
    void on_tabWidget_tabCloseRequested(int index);

    //! \brief widgetPdf معرفة الودجاة المفعلة
    //! \return الودجاة
    //!
    WidgetPdf *widgetPdf();

    //! \brief showTabBooks عرض لسان اختيار الكتب
    //!
    void showTabBooks();

    //! \brief wPdfEmitFirstPage الانتقال الى الضفحة الاولى من ملف
    //!
    void wPdfEmitFirstPage();

    //! \brief wPdfEmitPreviousPage الانتقال الى الصفحة السابقة
    //!
    void wPdfEmitPreviousPage();

    //! \brief wPdfEmitNextPage الانتقال الى الصفحة التالية
    //!
    void wPdfEmitNextPage();

    //! \brief wPdfEmitLastPage الانتقال الى الصفحة الاخيرة
    //!
    void wPdfEmitLastPage();

    //! \brief wPdfEmitAddBookmark اضافة مفضلة
    //!
    void wPdfEmitAddBookmark();

    //! \brief wPdfRefresh تحديث ملف بي دي اف
    //!
    void wPdfRefresh();

    //! \brief wPdfEmitZoomIn  تكبير ملف بي دي اف
    //!
    void wPdfEmitZoomIn();

    //! \brief wPdfEmitZoomOut تصغير ملف بي دي اف
    //!
    void wPdfEmitZoomOut();

    //! \brief wPdfEmitSetRubberBand تفعيل وضع التحديد في ملف بي دي اف
    //! \param arg
    //!
    void wPdfEmitSetRubberBand(bool arg);

    //! \brief wPdfEmitSetInvertColors عكس الالوان في ملف بي دي اف
    //! \param arg
    //!
    void wPdfEmitSetInvertColors(bool arg);

    //! \brief wPdfSetPage الانتقال الى صفحة في ملف بي دي اف
    //! \param arg
    //!
    void wPdfSetPage(int arg);

    //! \brief wPdfSetContinousMode وضع التتابع ملف بي دي اف
    //! \param arg
    //!
    void wPdfSetContinousMode(bool arg);

    //! \brief wPdfSetFiteWith عرض التتابع او صفحة واحدة
    //! \param arg
    //!
    void wPdfSetFiteWith(int arg);

    //! \brief wPdfSetLayoutMode طريقة عرض الصفحة
    //! \param arg
    //!
    void wPdfSetLayoutMode(int arg);

    //! \brief showHideSearch تفغيل الغاء البحث
    //!
    void showHideSearch();

    //! \brief on_labelTags_linkActivated تفعيل الوسم
    //! \param link
    //!
    void on_labelTags_linkActivated(const QString &link);

    //! \brief configure تحرير الخيارات
    //!
    void configure();
void fontModel(EditBookDialog *w);
void slotShowFullScreen(bool show);

bool chekForaddBook();
void addCurrentBook();
private:

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

    Ui::MainWindow *ui;

    enum ViewMode{
        IconView,
        DetailView
    };
    //!
    //! \brief mPath
    //!
   // Path *mPath;

    //! \brief mRatingWidget ودجات التنقيط
    //!
    RatingWidget *mRatingWidget;

    //! \brief mWidgetPdf ودجات الحاوية لملفات بي دي اف
    //!
    WidgetPdf *mWidgetPdf;

    //! \brief mWidgetBookmark ودجات المفضلات
    //!
    WidgetBookmark *mWidgetBookmark;

    //! \brief mModel قاعدة البيانات
    //!
    MyModel *mModel;

    //! \brief mSearchTools اداة البحث
    //!
    SearchTools *mSearchTools;

    //! \brief mActions كل الاوامر الخاصة بقائمة الكتب او البي دي اف
    //!
    Actions *mActions;

    ///  الاوامر المشتركة
    QAction *mActBooksTab;
    QAction *mActScanFolder;
    QAction *mActOpen;
    QAction *mActOpenWith;
    QAction *mActMoveToGroup;
    QAction *mActMoveToAuthor;
    QAction *mActAddBook;
     QAction *mActAddCurrent;
    QAction *mActAbout;
    QAction *mActFind;
    QAction *mActStandardMode;
    QAction *mActShowFullScreen;
 QAction *mActcleanBooks;

    /// عدد الملفات السابقة
    enum { MaxRecentFiles = 7 };
    QAction *recentFileActs[MaxRecentFiles];
    QMenu *menuRecent;
    QAction *mActRecents;

QAction *mActConfig;

    bool mDebug;
};

#endif // MAINWINDOW_H
