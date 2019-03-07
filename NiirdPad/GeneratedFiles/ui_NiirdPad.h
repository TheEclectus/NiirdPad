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
    QAction *actionNew_Project;
    QAction *actionNew_File;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionImport_Project;
    QAction *actionExport_Project;
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
        actionNew_Project = new QAction(NiirdPadClass);
        actionNew_Project->setObjectName(QString::fromUtf8("actionNew_Project"));
        actionNew_File = new QAction(NiirdPadClass);
        actionNew_File->setObjectName(QString::fromUtf8("actionNew_File"));
        actionOpen = new QAction(NiirdPadClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(NiirdPadClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave_As = new QAction(NiirdPadClass);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionImport_Project = new QAction(NiirdPadClass);
        actionImport_Project->setObjectName(QString::fromUtf8("actionImport_Project"));
        actionExport_Project = new QAction(NiirdPadClass);
        actionExport_Project->setObjectName(QString::fromUtf8("actionExport_Project"));
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
        menuFile->addAction(actionNew_Project);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionImport_Project);
        menuFile->addAction(actionExport_Project);

        retranslateUi(NiirdPadClass);

        QMetaObject::connectSlotsByName(NiirdPadClass);
    } // setupUi

    void retranslateUi(QMainWindow *NiirdPadClass)
    {
        NiirdPadClass->setWindowTitle(QApplication::translate("NiirdPadClass", "NiirdPad", nullptr));
        actionNew_Project->setText(QApplication::translate("NiirdPadClass", "New", nullptr));
        actionNew_File->setText(QApplication::translate("NiirdPadClass", "New File", nullptr));
        actionOpen->setText(QApplication::translate("NiirdPadClass", "Open", nullptr));
        actionSave->setText(QApplication::translate("NiirdPadClass", "Save", nullptr));
        actionSave_As->setText(QApplication::translate("NiirdPadClass", "Save As...", nullptr));
        actionImport_Project->setText(QApplication::translate("NiirdPadClass", "Import Project...", nullptr));
        actionExport_Project->setText(QApplication::translate("NiirdPadClass", "Export Project...", nullptr));
        menuFile->setTitle(QApplication::translate("NiirdPadClass", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NiirdPadClass: public Ui_NiirdPadClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NIIRDPAD_H
