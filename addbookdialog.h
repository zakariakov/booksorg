#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddBookDialog(QWidget *parent = 0);
    ~AddBookDialog();
    
private:
    Ui::AddBookDialog *ui;
};

#endif // ADDBOOKDIALOG_H
