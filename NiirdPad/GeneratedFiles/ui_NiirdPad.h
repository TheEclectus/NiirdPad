/********************************************************************************
** Form generated from reading UI file 'NiirdPad.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NIIRDPAD_H
#define UI_NIIRDPAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include "QSDLPanel.h"

QT_BEGIN_NAMESPACE

class Ui_NiirdPadClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSDLPanel *widget;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *NiirdPadClass)
    {
        if (NiirdPadClass->objectName().isEmpty())
            NiirdPadClass->setObjectName(QString::fromUtf8("NiirdPadClass"));
        NiirdPadClass->resize(600, 400);
        centralWidget = new QWidget(NiirdPadClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget = new QSDLPanel(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));

        horizontalLayout->addWidget(widget);

        NiirdPadClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(NiirdPadClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        NiirdPadClass->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());

        retranslateUi(NiirdPadClass);

        QMetaObject::connectSlotsByName(NiirdPadClass);
    } // setupUi

    void retranslateUi(QMainWindow *NiirdPadClass)
    {
        NiirdPadClass->setWindowTitle(QApplication::translate("NiirdPadClass", "NiirdPad", nullptr));
        menuFile->setTitle(QApplication::translate("NiirdPadClass", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NiirdPadClass: public Ui_NiirdPadClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NIIRDPAD_H
