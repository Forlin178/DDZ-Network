/********************************************************************************
** Form generated from reading UI file 'buttongroup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTONGROUP_H
#define UI_BUTTONGROUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include "D:\Qt_Project\LandLords2\mybutton.h"

QT_BEGIN_NAMESPACE

class Ui_ButtonGroup
{
public:
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *startPage;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    MyButton *btnStart;
    QSpacerItem *horizontalSpacer_2;
    QWidget *playCardPage;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    MyButton *btnPlayCard;
    QSpacerItem *horizontalSpacer_4;
    QWidget *passOrPlayPage;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    MyButton *btnPass;
    QSpacerItem *horizontalSpacer_6;
    MyButton *btnPlayCard_2;
    QSpacerItem *horizontalSpacer_7;
    QWidget *callLordPage;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    MyButton *btnGiveUP;
    QSpacerItem *horizontalSpacer_10;
    MyButton *btnOneBet;
    QSpacerItem *horizontalSpacer_11;
    MyButton *btnTwoBet;
    QSpacerItem *horizontalSpacer_12;
    MyButton *btnThreeBet;
    QSpacerItem *horizontalSpacer_9;
    QWidget *nullPage;

    void setupUi(QWidget *ButtonGroup)
    {
        if (ButtonGroup->objectName().isEmpty())
            ButtonGroup->setObjectName(QString::fromUtf8("ButtonGroup"));
        ButtonGroup->resize(516, 52);
        horizontalLayout = new QHBoxLayout(ButtonGroup);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(ButtonGroup);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        startPage = new QWidget();
        startPage->setObjectName(QString::fromUtf8("startPage"));
        horizontalLayout_2 = new QHBoxLayout(startPage);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(191, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnStart = new MyButton(startPage);
        btnStart->setObjectName(QString::fromUtf8("btnStart"));

        horizontalLayout_2->addWidget(btnStart);

        horizontalSpacer_2 = new QSpacerItem(191, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        stackedWidget->addWidget(startPage);
        playCardPage = new QWidget();
        playCardPage->setObjectName(QString::fromUtf8("playCardPage"));
        horizontalLayout_3 = new QHBoxLayout(playCardPage);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(191, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        btnPlayCard = new MyButton(playCardPage);
        btnPlayCard->setObjectName(QString::fromUtf8("btnPlayCard"));

        horizontalLayout_3->addWidget(btnPlayCard);

        horizontalSpacer_4 = new QSpacerItem(191, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        stackedWidget->addWidget(playCardPage);
        passOrPlayPage = new QWidget();
        passOrPlayPage->setObjectName(QString::fromUtf8("passOrPlayPage"));
        horizontalLayout_4 = new QHBoxLayout(passOrPlayPage);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_5 = new QSpacerItem(117, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        btnPass = new MyButton(passOrPlayPage);
        btnPass->setObjectName(QString::fromUtf8("btnPass"));

        horizontalLayout_4->addWidget(btnPass);

        horizontalSpacer_6 = new QSpacerItem(40, 15, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        btnPlayCard_2 = new MyButton(passOrPlayPage);
        btnPlayCard_2->setObjectName(QString::fromUtf8("btnPlayCard_2"));

        horizontalLayout_4->addWidget(btnPlayCard_2);

        horizontalSpacer_7 = new QSpacerItem(116, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        stackedWidget->addWidget(passOrPlayPage);
        callLordPage = new QWidget();
        callLordPage->setObjectName(QString::fromUtf8("callLordPage"));
        horizontalLayout_5 = new QHBoxLayout(callLordPage);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_8 = new QSpacerItem(12, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        btnGiveUP = new MyButton(callLordPage);
        btnGiveUP->setObjectName(QString::fromUtf8("btnGiveUP"));

        horizontalLayout_5->addWidget(btnGiveUP);

        horizontalSpacer_10 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_10);

        btnOneBet = new MyButton(callLordPage);
        btnOneBet->setObjectName(QString::fromUtf8("btnOneBet"));

        horizontalLayout_5->addWidget(btnOneBet);

        horizontalSpacer_11 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_11);

        btnTwoBet = new MyButton(callLordPage);
        btnTwoBet->setObjectName(QString::fromUtf8("btnTwoBet"));

        horizontalLayout_5->addWidget(btnTwoBet);

        horizontalSpacer_12 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_12);

        btnThreeBet = new MyButton(callLordPage);
        btnThreeBet->setObjectName(QString::fromUtf8("btnThreeBet"));

        horizontalLayout_5->addWidget(btnThreeBet);

        horizontalSpacer_9 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_9);

        stackedWidget->addWidget(callLordPage);
        nullPage = new QWidget();
        nullPage->setObjectName(QString::fromUtf8("nullPage"));
        stackedWidget->addWidget(nullPage);

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(ButtonGroup);

        stackedWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(ButtonGroup);
    } // setupUi

    void retranslateUi(QWidget *ButtonGroup)
    {
        ButtonGroup->setWindowTitle(QCoreApplication::translate("ButtonGroup", "Form", nullptr));
        btnStart->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        btnPlayCard->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        btnPass->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        btnPlayCard_2->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        btnGiveUP->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        btnOneBet->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        btnTwoBet->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
        btnThreeBet->setText(QCoreApplication::translate("ButtonGroup", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ButtonGroup: public Ui_ButtonGroup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTONGROUP_H
