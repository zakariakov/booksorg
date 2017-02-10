#ifndef POPUPFORM_H
#define POPUPFORM_H

#include <QWidget>

namespace Ui {
class PopupForm;
}

class PopupForm : public QWidget
{
    Q_OBJECT

public:
    explicit PopupForm(QWidget *parent = 0);
    ~PopupForm();

private:
    Ui::PopupForm *ui;
};

#endif // POPUPFORM_H
