/********************************************************************************
** Form generated from reading UI file 'gamemode.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEMODE_H
#define UI_GAMEMODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameMode
{
public:
    QVBoxLayout *verticalLayout_6;
    QStackedWidget *stackedWidget;
    QWidget *mode_page;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout;
    QLabel *singlemode_label;
    QPushButton *singlemode_btn;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *netmode_label;
    QPushButton *netmode_btn;
    QSpacerItem *horizontalSpacer_5;
    QWidget *room_page;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QLabel *auto_label;
    QPushButton *auto_btn;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_6;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_3;
    QLabel *manual_label;
    QPushButton *manual_btn;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_7;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_5;
    QLabel *search_label;
    QPushButton *search_btn;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *horizontalSpacer_2;
    QLabel *information;

    void setupUi(QDialog *GameMode)
    {
        if (GameMode->objectName().isEmpty())
            GameMode->setObjectName(QString::fromUtf8("GameMode"));
        GameMode->resize(736, 547);
        verticalLayout_6 = new QVBoxLayout(GameMode);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        stackedWidget = new QStackedWidget(GameMode);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        mode_page = new QWidget();
        mode_page->setObjectName(QString::fromUtf8("mode_page"));
        horizontalLayout = new QHBoxLayout(mode_page);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        singlemode_label = new QLabel(mode_page);
        singlemode_label->setObjectName(QString::fromUtf8("singlemode_label"));
        singlemode_label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(singlemode_label);

        singlemode_btn = new QPushButton(mode_page);
        singlemode_btn->setObjectName(QString::fromUtf8("singlemode_btn"));

        verticalLayout->addWidget(singlemode_btn);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        netmode_label = new QLabel(mode_page);
        netmode_label->setObjectName(QString::fromUtf8("netmode_label"));
        netmode_label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(netmode_label);

        netmode_btn = new QPushButton(mode_page);
        netmode_btn->setObjectName(QString::fromUtf8("netmode_btn"));

        verticalLayout_2->addWidget(netmode_btn);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        stackedWidget->addWidget(mode_page);
        room_page = new QWidget();
        room_page->setObjectName(QString::fromUtf8("room_page"));
        horizontalLayout_2 = new QHBoxLayout(room_page);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        auto_label = new QLabel(room_page);
        auto_label->setObjectName(QString::fromUtf8("auto_label"));
        auto_label->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(auto_label);

        auto_btn = new QPushButton(room_page);
        auto_btn->setObjectName(QString::fromUtf8("auto_btn"));

        verticalLayout_3->addWidget(auto_btn);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(verticalLayout_3);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        manual_label = new QLabel(room_page);
        manual_label->setObjectName(QString::fromUtf8("manual_label"));
        manual_label->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(manual_label);

        manual_btn = new QPushButton(room_page);
        manual_btn->setObjectName(QString::fromUtf8("manual_btn"));

        verticalLayout_4->addWidget(manual_btn);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_4);


        horizontalLayout_2->addLayout(verticalLayout_4);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_5);

        search_label = new QLabel(room_page);
        search_label->setObjectName(QString::fromUtf8("search_label"));
        search_label->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(search_label);

        search_btn = new QPushButton(room_page);
        search_btn->setObjectName(QString::fromUtf8("search_btn"));

        verticalLayout_5->addWidget(search_btn);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_6);


        horizontalLayout_2->addLayout(verticalLayout_5);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        stackedWidget->addWidget(room_page);

        verticalLayout_6->addWidget(stackedWidget);

        information = new QLabel(GameMode);
        information->setObjectName(QString::fromUtf8("information"));

        verticalLayout_6->addWidget(information);


        retranslateUi(GameMode);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(GameMode);
    } // setupUi

    void retranslateUi(QDialog *GameMode)
    {
        GameMode->setWindowTitle(QCoreApplication::translate("GameMode", "Dialog", nullptr));
        singlemode_label->setText(QCoreApplication::translate("GameMode", "\345\215\225\346\234\272\346\250\241\345\274\217", nullptr));
        singlemode_btn->setText(QCoreApplication::translate("GameMode", "PushButton", nullptr));
        netmode_label->setText(QCoreApplication::translate("GameMode", "\350\201\224\346\234\272\346\250\241\345\274\217", nullptr));
        netmode_btn->setText(QCoreApplication::translate("GameMode", "PushButton", nullptr));
        auto_label->setText(QCoreApplication::translate("GameMode", "\350\207\252\345\212\250\345\210\233\345\273\272", nullptr));
        auto_btn->setText(QCoreApplication::translate("GameMode", "PushButton", nullptr));
        manual_label->setText(QCoreApplication::translate("GameMode", "\346\211\213\345\212\250\345\210\233\345\273\272", nullptr));
        manual_btn->setText(QCoreApplication::translate("GameMode", "PushButton", nullptr));
        search_label->setText(QCoreApplication::translate("GameMode", "\346\220\234\347\264\242\346\210\277\351\227\264", nullptr));
        search_btn->setText(QCoreApplication::translate("GameMode", "PushButton", nullptr));
        information->setText(QCoreApplication::translate("GameMode", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameMode: public Ui_GameMode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEMODE_H
