#ifndef BOOKS_ADAPTOR_H
#define BOOKS_ADAPTOR_H
#include <QtDBus>

class Books_Adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.elokab.books")
    Q_CLASSINFO("D-Bus Introspection", ""
                "  <interface name=\"org.elokab.books\">\n"
                "    <method name=\"slotOpenFileName\">\n"
                     "<arg name=\"fileName\" type=\"s\" direction=\"in\"/>"
                "    </method>"
                 "  </interface>\n"
                "")

public:
    Books_Adaptor(QObject *parent);

public Q_SLOTS: // METHODS

    void slotOpenFileName(const QString &fileName);

};

#endif // BOOKS_ADAPTOR_H
