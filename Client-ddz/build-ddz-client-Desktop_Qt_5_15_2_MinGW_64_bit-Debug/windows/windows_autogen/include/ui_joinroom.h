/********************************************************************************
** Form generated from reading UI file 'joinroom.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JOINROOM_H
#define UI_JOINROOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JoinRoom
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *describe;
    QLineEdit *lineEdit;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *create_btn;
    QSpacerItem *horizontalSpacer_2;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *search_btn;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *join_btn;
    QSpacerItem *horizontalSpacer_5;

    void setupUi(QDialog *JoinRoom)
    {
        if (JoinRoom->objectName().isEmpty())
            JoinRoom->setObjectName(QString::fromUtf8("JoinRoom"));
        JoinRoom->resize(256, 139);
        verticalLayout = new QVBoxLayout(JoinRoom);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        describe = new QLabel(JoinRoom);
        describe->setObjectName(QString::fromUtf8("describe"));

        verticalLayout->addWidget(describe);

        lineEdit = new QLineEdit(JoinRoom);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        stackedWidget = new QStackedWidget(JoinRoom);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        horizontalLayout = new QHBoxLayout(page);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(56, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        create_btn = new QPushButton(page);
        create_btn->setObjectName(QString::fromUtf8("create_btn"));

        horizontalLayout->addWidget(create_btn);

        horizontalSpacer_2 = new QSpacerItem(56, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        horizontalLayout_3 = new QHBoxLayout(page_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(5, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        search_btn = new QPushButton(page_2);
        search_btn->setObjectName(QString::fromUtf8("search_btn"));

        horizontalLayout_3->addWidget(search_btn);

        horizontalSpacer_4 = new QSpacerItem(5, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        join_btn = new QPushButton(page_2);
        join_btn->setObjectName(QString::fromUtf8("join_btn"));

        horizontalLayout_3->addWidget(join_btn);

        horizontalSpacer_5 = new QSpacerItem(5, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(JoinRoom);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(JoinRoom);
    } // setupUi

    void retranslateUi(QDialog *JoinRoom)
    {
        JoinRoom->setWindowTitle(QCoreApplication::translate("JoinRoom", "Dialog", nullptr));
        describe->setText(QCoreApplication::translate("JoinRoom", "TextLabel", nullptr));
        create_btn->setText(QCoreApplication::translate("JoinRoom", "\345\210\233\345\273\272\346\210\277\351\227\264", nullptr));
        search_btn->setText(QCoreApplication::translate("JoinRoom", "\346\220\234\347\264\242\346\210\277\351\227\264", nullptr));
        join_btn->setText(QCoreApplication::translate("JoinRoom", "\345\212\240\345\205\245\346\210\277\351\227\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JoinRoom: public Ui_JoinRoom {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JOINROOM_H
