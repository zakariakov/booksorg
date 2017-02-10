#include "standarditem.h"

StandardItem::StandardItem(const QString &iconName)
{
setIcon(QIcon(iconName));
ChargeThread *charge=new ChargeThread(iconName);

//QObject::connect(charge,SIGNAL(iconFinished(QIcon)),this,SLOT(setNewIcon(QIcon)));
//QMetaObject::connect(charge,SIGNAL(iconFinished(QIcon)),this,SLOT(setNewIcon(QIcon)));

}




ChargeThread::ChargeThread(const QString &iconName,QObject *parent)
{

}

void ChargeThread:: run()
{

}
