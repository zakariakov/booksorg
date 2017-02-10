#include "addbookdialog.h"
#include "ui_addbookdialog.h"
#include <QFile>
AddBookDialog::AddBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookDialog)
{
    ui->setupUi(this);
}

AddBookDialog::~AddBookDialog()
{
    delete ui;
}
