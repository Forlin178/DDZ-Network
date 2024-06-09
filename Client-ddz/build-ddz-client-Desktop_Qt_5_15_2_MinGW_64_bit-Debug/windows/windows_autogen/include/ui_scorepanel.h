/********************************************************************************
** Form generated from reading UI file 'scorepanel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCOREPANEL_H
#define UI_SCOREPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScorePanel
{
public:
    QGridLayout *gridLayout;
    QLabel *labunit_3;
    QLabel *labscore_2;
    QLabel *labunit_2;
    QLabel *labscore_3;
    QLabel *labscore_1;
    QLabel *rightName;
    QLabel *myName;
    QLabel *leftName;
    QLabel *labunit_1;

    void setupUi(QWidget *ScorePanel)
    {
        if (ScorePanel->objectName().isEmpty())
            ScorePanel->setObjectName(QString::fromUtf8("ScorePanel"));
        ScorePanel->resize(241, 135);
        ScorePanel->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(ScorePanel);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labunit_3 = new QLabel(ScorePanel);
        labunit_3->setObjectName(QString::fromUtf8("labunit_3"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        font.setBold(true);
        labunit_3->setFont(font);
        labunit_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labunit_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labunit_3, 2, 2, 1, 1);

        labscore_2 = new QLabel(ScorePanel);
        labscore_2->setObjectName(QString::fromUtf8("labscore_2"));
        labscore_2->setFont(font);
        labscore_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labscore_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labscore_2, 1, 1, 1, 1);

        labunit_2 = new QLabel(ScorePanel);
        labunit_2->setObjectName(QString::fromUtf8("labunit_2"));
        labunit_2->setFont(font);
        labunit_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labunit_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labunit_2, 1, 2, 1, 1);

        labscore_3 = new QLabel(ScorePanel);
        labscore_3->setObjectName(QString::fromUtf8("labscore_3"));
        labscore_3->setFont(font);
        labscore_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labscore_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labscore_3, 2, 1, 1, 1);

        labscore_1 = new QLabel(ScorePanel);
        labscore_1->setObjectName(QString::fromUtf8("labscore_1"));
        labscore_1->setFont(font);
        labscore_1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labscore_1->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labscore_1, 0, 1, 1, 1);

        rightName = new QLabel(ScorePanel);
        rightName->setObjectName(QString::fromUtf8("rightName"));
        rightName->setFont(font);
        rightName->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        rightName->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(rightName, 2, 0, 1, 1);

        myName = new QLabel(ScorePanel);
        myName->setObjectName(QString::fromUtf8("myName"));
        myName->setFont(font);
        myName->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        myName->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(myName, 0, 0, 1, 1);

        leftName = new QLabel(ScorePanel);
        leftName->setObjectName(QString::fromUtf8("leftName"));
        leftName->setFont(font);
        leftName->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        leftName->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(leftName, 1, 0, 1, 1);

        labunit_1 = new QLabel(ScorePanel);
        labunit_1->setObjectName(QString::fromUtf8("labunit_1"));
        labunit_1->setFont(font);
        labunit_1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        labunit_1->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labunit_1, 0, 2, 1, 1);


        retranslateUi(ScorePanel);

        QMetaObject::connectSlotsByName(ScorePanel);
    } // setupUi

    void retranslateUi(QWidget *ScorePanel)
    {
        ScorePanel->setWindowTitle(QCoreApplication::translate("ScorePanel", "Form", nullptr));
        labunit_3->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
        labscore_2->setText(QCoreApplication::translate("ScorePanel", "0", nullptr));
        labunit_2->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
        labscore_3->setText(QCoreApplication::translate("ScorePanel", "0", nullptr));
        labscore_1->setText(QCoreApplication::translate("ScorePanel", "0", nullptr));
        rightName->setText(QCoreApplication::translate("ScorePanel", "\345\217\263\344\272\272\346\234\272", nullptr));
        myName->setText(QCoreApplication::translate("ScorePanel", "\346\210\221", nullptr));
        leftName->setText(QCoreApplication::translate("ScorePanel", "\345\267\246\344\272\272\346\234\272", nullptr));
        labunit_1->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScorePanel: public Ui_ScorePanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCOREPANEL_H
