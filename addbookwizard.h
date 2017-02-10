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

#ifndef ADDBOOKWIZARD_H
#define ADDBOOKWIZARD_H

#include <QWizard>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QTextEdit>
#include <QComboBox>
class OutputFilesPage;
class AddBookWizard : public QWizard
{
    Q_OBJECT
public:
  explicit AddBookWizard(const QStringList &groups,const QStringList &autors,
                         const QString &file=QString(),QWidget *parent = 0);
  void setPixmapName(QString name){mPixName=name;}

void accept();
QMap<QString,QString> getNewBook(){return mMap;}
QStringList mGroups;
QStringList mAuthors;

signals:

public slots:

 private:
    QString mPixName;
    QString mDir;
 OutputFilesPage *mOutputFilesPage;

    QMap<QString,QString>mMap;
};


class ClassInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    ClassInfoPage(QWidget *parent = 0);

private:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
     QLabel *label2;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
private slots:
    void openFile();
};
class OutputFilesPage : public QWizardPage
{
     Q_OBJECT

 public:
     OutputFilesPage(AddBookWizard *p,const QString &file=QString() ,QWidget *parent = 0);
     QString getAuthor(){return comboBoxAuthors->currentText();}
     QString getGroup(){return comboBoxGroups->currentText();}
     QString getInfo(){return textEditProperties->toPlainText().remove("'");}
QString getFilePath(){return mFile;}
 protected:
     void initializePage();

     // QAction *mActDelete;
     // QAction *mActMove;
 private:
     QGridLayout *gridLayout;
     QLabel *label_2;
     QLineEdit *lineEditTitle;
     QLabel *toolButtonImage;
     QLabel *label_3;
     QLineEdit *lineEditComment;
     QLabel *label_4;
     QLabel *label_5;
       QLabel *label_6;
     QTextEdit *textEditProperties;
     QLineEdit *lineEditAuthor;

     QComboBox *comboBoxAuthors;
QComboBox *comboBoxGroups;

AddBookWizard *mparent;
QString mFile;
};

#endif // ADDBOOKWIZARD_H
