#include "configuredialog.h"
#include "ui_configuredialog.h"
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
ConfigureDialog::ConfigureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDialog)
{
    ui->setupUi(this);
    QSettings settings;

    ui->lineEditPath->setText(settings.value("BooksPath",
                                             QDir::homePath()+"/."+QApplication::applicationName()).toString());
    ui->checkBoxCopy->setChecked(settings.value("CopyBooks",false).toBool());

    ui->lineEditExPrg->setText(settings.value("ExternalProgram").toString());


}

ConfigureDialog::~ConfigureDialog()
{
    delete ui;
}




void ConfigureDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button)==QDialogButtonBox::Ok)
    {
        QFileInfo fi(ui->lineEditPath->text());
        if(fi.isWritable()){
         QSettings settings;
         settings.setValue("BooksPath",ui->lineEditPath->text());
         settings.setValue("CopyBooks",ui->checkBoxCopy->isChecked());
         settings.setValue("ExternalProgram",ui->lineEditExPrg->text());
         this->accept();
        }else{
            QMessageBox::information(this,"",tr("this directory %1 .\n is read only").arg(ui->lineEditPath->text()));
            ui->lineEditPath->setFocus();
            return;
        }
    }
}

void ConfigureDialog::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     ui->lineEditPath->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
         ui->lineEditPath->setText(dir);
}
