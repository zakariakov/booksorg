#ifndef STANDARDITEM_H
#define STANDARDITEM_H
#include <QObject>
#include <QStandardItem>
#include <QThread>

class ChargeThread : public QThread
{
    Q_OBJECT
public:
    explicit ChargeThread(const QString &iconName,QObject *parent = 0);
void run();
signals:
void iconFinished(QIcon);

public slots:

};

class StandardItem : public QStandardItem
{

public:
    StandardItem(const QString &iconName);
private slots:
  void  setNewIcon(QIcon icon){setIcon( icon);}


};

#endif // STANDARDITEM_H
