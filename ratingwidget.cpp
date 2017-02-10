#include "ratingwidget.h"
#include <QToolButton>
#include <QLayout>
#include <QLabel>
#include <QDebug>

RatingWidget::RatingWidget(bool saveDlg, QWidget *parent) :
    QWidget(parent),mShowSave(saveDlg)
{

    QGridLayout *gridLayout = new QGridLayout(this);
    //  horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    //   QLabel*  labelStars = new QLabel(tr("Rating :"),this);
    //    labelStars->setObjectName(QStringLiteral("labelStars"));


    //    horizontalLayout->addWidget(labelStars);

    mWidgetStars = new QWidget(this);
    mWidgetStars->setObjectName("widgetStars");
    mWidgetStars->setMinimumSize(QSize(80, 16));
    mWidgetStars->setMaximumSize(QSize(80, 16));
    mWidgetStars->setCursor(QCursor(Qt::PointingHandCursor));
    mWidgetStars->setLayoutDirection(Qt::LeftToRight);
    mWidgetStars->setStyleSheet(QLatin1String("QWidget#widgetStars{border-image: url(:/icons/0.png);}\n"
                                              ""));
    gridLayout->addWidget(mWidgetStars,0,0,Qt::AlignCenter);

    QHBoxLayout *hLayoutButtons = new QHBoxLayout(mWidgetStars);
    hLayoutButtons->setContentsMargins(0,0,0,0);

    for (int i = 0; i < 5; ++i) {
        QToolButton *btn=new QToolButton;
        btn->setObjectName("btn_"+QString::number(i+1));
        //  qDebug()<<btn->objectName();
        btn->setMinimumSize(16,16);
        btn->setMaximumSize(16,16);

        btn->setStyleSheet("QToolButton{background-color:transparent;border-color: transparent;border-radius:0;}");

        hLayoutButtons->addWidget(btn);
        connect(btn,SIGNAL(clicked()),this,SLOT(newRating()));

    }

    mButtonBox = new QDialogButtonBox(this);
    //mButtonBox->setObjectName(QStringLiteral("buttonBox"));
    mButtonBox->setCenterButtons(true);
    mButtonBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    mButtonBox->setOrientation(Qt::Horizontal);
    mButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);
    connect(mButtonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(buttonBoxClicked(QAbstractButton*)));
    gridLayout->addWidget(mButtonBox,1,0,Qt::AlignCenter);
    mButtonBox->setVisible(false);


    clearRating();
}
void RatingWidget::setRating(int r, QString file)
{
    qDebug()<<"RatingWidget::setRating"<<file<<r;
    mRating=r;
    oldRating=r;
    mFile=file;

    setRatingImage(r);
    mButtonBox->setVisible(false);

}

//________________________________________________________________
void RatingWidget::clearRating()
{

    mRating=0;
    mFile=QString();

    mWidgetStars->setStyleSheet( QString("QWidget#widgetStars{}"));

    mButtonBox->setVisible(false);

}

//________________________________________________________________
void RatingWidget::newRating()
{
    QToolButton *action = qobject_cast<QToolButton *>(sender());
    if(action)
    {

        QString s=action->objectName().section("_",1,1);
        if(mRating==s.toInt()){
            mRating=mRating-1;
        }else{
            mRating=s.toInt();
        }
        setRatingImage(mRating);

        if(mShowSave)
            mButtonBox->setVisible(true);
    }

}

//________________________________________________________________
void RatingWidget::setRatingImage(int r)
{
    mWidgetStars->setStyleSheet(
                QString("QWidget#widgetStars{border-image: url(:/icons/%1.png);}")
                .arg(QString::number(r)));
  qDebug()<<  "RatingWidget::setRatingImage"<<r;
}

//________________________________________________________________
void RatingWidget::buttonBoxClicked(QAbstractButton* button)
{
    mButtonBox->setVisible(false);
    if(mButtonBox->standardButton(button)==QDialogButtonBox::Save){
        oldRating=mRating;
        emit ratingChanged(mRating,mFile);
    }else{
        mRating=oldRating;
        setRatingImage(oldRating);
    }


   // qDebug()<<"emit saveRatingWidget"<<mFile<<mRating;
}

