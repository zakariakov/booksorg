#ifndef SCANFOLDER_H
#define SCANFOLDER_H

#include <QWizard>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QProgressBar>
class ScanFolder : public QWizard
{
    Q_OBJECT
public:
    explicit ScanFolder(QWidget *parent = 0);
    
signals:
    
public slots:
    
};
class ClassIntroScan : public QWizardPage
{
    Q_OBJECT

public:
    ClassIntroScan(QWidget *parent = 0);

private:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
     QLabel *label2;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
private slots:
    void openFolder();

};

class ClassProgress : public QWizardPage
{
    Q_OBJECT

public:
    ClassProgress(QWidget *parent = 0);
protected:
    //void initializePage();
    void initializePage () ;
    void setVisible(bool visible);

private:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
     QLabel *label2;
    QProgressBar *progressBar;
    bool mStop;
private slots:

void creatPdfs();
};
#endif // SCANFOLDER_H
