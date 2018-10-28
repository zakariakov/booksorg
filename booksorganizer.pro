#-------------------------------------------------
#
# Project created by QtCreator 2015-03-10T11:19:51
#
#-------------------------------------------------

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT +=concurrent widgets printsupport

unix{
 QT += dbus
}
#qtHaveModule(dbus){

#}

TARGET = booksorg
DESTDIR =bin
TEMPLATE = app

CONFIG += qt \
          release
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build


win32{
    INCLUDEPATH  += poppler-win\include\poppler-qt5
    INCLUDEPATH  += poppler-win\include\poppler-cpp
    LIBS         += -L poppler-win\bin -lpoppler-qt5
    RC_FILE = myapp.rc
#    QT +=winextras
}else{

    equals(QT_MAJOR_VERSION, 5) {
    INCLUDEPATH  += /usr/include/poppler/qt5
    LIBS         += -L/usr/lib -lpoppler-qt5
    message("qt vertion is :5 " $$QT_MAJOR_VERSION)
    }

    equals(QT_MAJOR_VERSION, 4) {
    INCLUDEPATH  += /usr/include/poppler/qt4
    LIBS         += -L/usr/lib -lpoppler-qt4
    message("qt vertion is :4 " $$QT_MAJOR_VERSION)
    }

}

SOURCES += main.cpp\
    mainwindow.cpp \
    bookinfo.cpp \
    addbookwizard.cpp \
    editbookdialog.cpp \
    addfolderwizard.cpp \
    documentview.cpp \
    searchthread.cpp \
    pageitem.cpp \
    mymodel.cpp \
    widgetpdf.cpp \
    itemdelegate.cpp \
    searchtools.cpp \
    addbookmarkdlg.cpp \
    ratingwidget.cpp \
    widgetbookmark.cpp \
    actions.cpp \
    configuredialog.cpp \
    path.cpp \
    thumbnail.cpp \
    books_adaptor.cpp

HEADERS  += mainwindow.h \
    bookinfo.h \
    addbookwizard.h \
    editbookdialog.h \
    addfolderwizard.h \
    searchthread.h \
    pageitem.h \
    documentview.h \
    mymodel.h \
    widgetpdf.h \
    itemdelegate.h \
    eicon.h \
    searchtools.h \
    addbookmarkdlg.h \
    ratingwidget.h \
    widgetbookmark.h \
    actions.h \
    configuredialog.h \
    path.h \
    thumbnail.h \
    books_adaptor.h

FORMS    += mainwindow.ui \
    editbookdialog.ui \
    addfolderwizard.ui \
    widgetpdf.ui \
    searchtools.ui \
    addbookmarkdlg.ui \
    configuredialog.ui

RESOURCES +=images.qrc


TRANSLATIONS    =translations/booksorg_ar.ts\
                 translations/booksorg_en.ts\
                 translations/booksorg_fr.ts\
CODECFORTR = UTF-8

# INSTALL-----------------------------------------------------------

 MKDIR = mkdir -p /usr/share/icons/hicolor/scalable/apps

target.path = /usr/bin

applicationsData.files=booksorg.desktop
applicationsData.path=/usr/share/applications/

icon.files=icons/booksorg.svg
icon.path=/usr/share/icons/hicolor/scalable/apps


INSTALLS += target \
             applicationsData\
             icon

DISTFILES += \
             fonts/fontawesome.ttf
