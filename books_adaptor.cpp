#include "books_adaptor.h"

Books_Adaptor::Books_Adaptor(QObject *parent)
: QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);

}

void Books_Adaptor::slotOpenFileName(const QString &fileName)
{
    QMetaObject::invokeMethod(parent(), "slotOpenFileName", Q_ARG(QString, fileName));

}
