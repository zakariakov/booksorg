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
#include "mainwindow.h"
#include "books_adaptor.h"
#include <QApplication>

#ifdef Q_OS_WIN32
static void associateFileTypes(const QStringList &fileTypes)
{
    QString displayName = QGuiApplication::applicationDisplayName();
    QString filePath = QCoreApplication::applicationFilePath();
    QString fileName = QFileInfo(filePath).fileName();

    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes\\Applications\\" + fileName, QSettings::NativeFormat);
    settings.setValue("FriendlyAppName", displayName);

    settings.beginGroup("SupportedTypes");
    foreach (const QString& fileType, fileTypes)
        settings.setValue(fileType, QString());
    settings.endGroup();

    settings.beginGroup("shell");
    settings.beginGroup("open");
    settings.setValue("FriendlyAppName", displayName);
    settings.beginGroup("Command");
    settings.setValue(".", QChar('"') + QDir::toNativeSeparators(filePath) + QString("\" \"%1\""));
}
#else
#include <QDBusConnection>

#endif

int main(int argc, char *argv[])
{

    qRegisterMetaType< Poppler::Page::Rotation >("Poppler::Page::Rotation");
    qRegisterMetaType< QList< QRectF > >("QList<QRectF>");

    QApplication a(argc, argv);
    a.setApplicationName("booksorg");
    a.setOrganizationName("elokab");
    a.setApplicationVersion("0.3");
    a.setWindowIcon(QIcon(":/icons/booksorg"));



    QSettings settings;
    settings.beginGroup("Main");
    QString locale= settings.value("Lang", "Default").toString();
    settings.endGroup();

    if(locale=="Default")
        locale = QLocale::system().name().section("_",0,0);

    /// اللغة الحالية لجميع البرنامج
    QLocale::setDefault(QLocale(locale));
    /// جلب ترجمات كيوتي
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += locale;
    QTranslator *translatorsys = new QTranslator;
    if (translatorsys->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(translatorsys);

    /// جلب ترجمة البرنامج من مجلد البرنامج
    QTranslator translator;
    translator.load(":/translations/booksorg_"+locale);
    a.installTranslator(&translator);

    QFontDatabase::addApplicationFont(":/fonts/fontawesome.ttf");

//    if(locale=="ar")
//        a.setLayoutDirection(Qt::RightToLeft);


#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    QLocale lx=QLocale(locale);
    a.setLayoutDirection(lx.textDirection());
    a.setApplicationDisplayName(QObject::tr("Books Organizer"));
#endif



#ifdef Q_OS_WIN32

    associateFileTypes(QStringList(".pdf"));

#else

    QDBusConnection connection = QDBusConnection::sessionBus();
    if (! QDBusConnection::sessionBus().registerService("org.elokab.books"))
    {
        qWarning() << "Unable to register 'org.elokab.books' service - is another instance of elokab-run running?";

        QDBusInterface dbus("org.elokab.books",
                            "/org/elokab/books",
                            "org.elokab.books");

        if (!dbus.isValid()) {
            qDebug() << "QDBusInterface 'org.elokab.books'  is not valid!";
            return 1;
        }

        QStringList args = a.arguments();

        if(args.count()>1)
        {
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");

            QUrl url = args.at(1);
            QByteArray encodedString =QByteArray::fromPercentEncoding(url.toString().toUtf8());
            QString string = codec->toUnicode(encodedString);

            QFileInfo fi(string);

            if (fi.exists() && fi.suffix().toLower()=="pdf" )
            {
               dbus.call(QDBus::NoBlock,"slotOpenFileName",fi.filePath());

            }



        }

        return 1;
    }


#endif
    QDir dir;
    dir.mkpath(QDir::homePath()+"/."+a.applicationName()+"/thumbnails");
    dir.mkpath(QDir::homePath()+"/."+a.applicationName()+"/books");

    MainWindow *w=new MainWindow;

#ifdef Q_OS_UNIX
    new Books_Adaptor(w);
    connection.registerObject("/org/elokab/books", w);
#endif

      w->show();
    
    return a.exec();
}
