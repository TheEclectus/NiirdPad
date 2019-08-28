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
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "QNodeView.h"

QT_BEGIN_NAMESPACE

class Ui_NiirdPadClass
{
public:
    QAction *actionNew_Project;
    QAction *actionNew_File;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionImportProject;
    QAction *actionExport_Project;
    QAction *actionEditReferenceWindowNew;
    QAction *actionEditReferenceWindowEdit;
    QAction *actionImportConsole;
    QAction *actionReference;
    QAction *actionDatabase;
    QAction *actionReload;
    QAction *actionReload_2;
    QAction *actionEditScriptWindow_Dialogue;
    QAction *actionEditScriptWindowDialogue;
    QAction *actionEditScriptWindowOption;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QNodeView *widget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuDebug;
    QMenu *menuEdit_Reference_Window;
    QMenu *menuEditScriptWindow;
    QMenu *menuTUScript;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *NiirdPadClass)
    {
        if (NiirdPadClass->objectName().isEmpty())
            NiirdPadClass->setObjectName(QString::fromUtf8("NiirdPadClass"));
        NiirdPadClass->resize(1043, 644);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/NiirdPad/Resources/Icon2.ico"), QSize(), QIcon::Normal, QIcon::Off);
        NiirdPadClass->setWindowIcon(icon);
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
        actionImportProject = new QAction(NiirdPadClass);
        actionImportProject->setObjectName(QString::fromUtf8("actionImportProject"));
        actionExport_Project = new QAction(NiirdPadClass);
        actionExport_Project->setObjectName(QString::fromUtf8("actionExport_Project"));
        actionEditReferenceWindowNew = new QAction(NiirdPadClass);
        actionEditReferenceWindowNew->setObjectName(QString::fromUtf8("actionEditReferenceWindowNew"));
        actionEditReferenceWindowEdit = new QAction(NiirdPadClass);
        actionEditReferenceWindowEdit->setObjectName(QString::fromUtf8("actionEditReferenceWindowEdit"));
        actionImportConsole = new QAction(NiirdPadClass);
        actionImportConsole->setObjectName(QString::fromUtf8("actionImportConsole"));
        actionReference = new QAction(NiirdPadClass);
        actionReference->setObjectName(QString::fromUtf8("actionReference"));
        actionDatabase = new QAction(NiirdPadClass);
        actionDatabase->setObjectName(QString::fromUtf8("actionDatabase"));
        actionReload = new QAction(NiirdPadClass);
        actionReload->setObjectName(QString::fromUtf8("actionReload"));
        actionReload_2 = new QAction(NiirdPadClass);
        actionReload_2->setObjectName(QString::fromUtf8("actionReload_2"));
        actionEditScriptWindow_Dialogue = new QAction(NiirdPadClass);
        actionEditScriptWindow_Dialogue->setObjectName(QString::fromUtf8("actionEditScriptWindow_Dialogue"));
        actionEditScriptWindowDialogue = new QAction(NiirdPadClass);
        actionEditScriptWindowDialogue->setObjectName(QString::fromUtf8("actionEditScriptWindowDialogue"));
        actionEditScriptWindowOption = new QAction(NiirdPadClass);
        actionEditScriptWindowOption->setObjectName(QString::fromUtf8("actionEditScriptWindowOption"));
        centralWidget = new QWidget(NiirdPadClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(4);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(200, 0));
        frame->setMaximumSize(QSize(16777215, 16777215));
        frame->setSizeIncrement(QSize(0, 0));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Plain);

        horizontalLayout->addWidget(frame);

        widget = new QNodeView(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(20);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(widget);

        NiirdPadClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(NiirdPadClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1043, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuDebug = new QMenu(menuBar);
        menuDebug->setObjectName(QString::fromUtf8("menuDebug"));
        menuEdit_Reference_Window = new QMenu(menuDebug);
        menuEdit_Reference_Window->setObjectName(QString::fromUtf8("menuEdit_Reference_Window"));
        menuEditScriptWindow = new QMenu(menuDebug);
        menuEditScriptWindow->setObjectName(QString::fromUtf8("menuEditScriptWindow"));
        menuTUScript = new QMenu(menuBar);
        menuTUScript->setObjectName(QString::fromUtf8("menuTUScript"));
        NiirdPadClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(NiirdPadClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        NiirdPadClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuDebug->menuAction());
        menuBar->addAction(menuTUScript->menuAction());
        menuFile->addAction(actionNew_Project);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionImportProject);
        menuFile->addAction(actionExport_Project);
        menuDebug->addAction(menuEdit_Reference_Window->menuAction());
        menuDebug->addAction(menuEditScriptWindow->menuAction());
        menuEdit_Reference_Window->addAction(actionEditReferenceWindowNew);
        menuEdit_Reference_Window->addAction(actionEditReferenceWindowEdit);
        menuEditScriptWindow->addAction(actionEditScriptWindowOption);
        menuEditScriptWindow->addAction(actionEditScriptWindowDialogue);
        menuTUScript->addAction(actionDatabase);
        menuTUScript->addAction(actionReload_2);
        menuTUScript->addAction(actionReload);

        retranslateUi(NiirdPadClass);

        QMetaObject::connectSlotsByName(NiirdPadClass);
    } // setupUi

    void retranslateUi(QMainWindow *NiirdPadClass)
    {
        NiirdPadClass->setWindowTitle(QApplication::translate("NiirdPadClass", "NiirdPad", nullptr));
        actionNew_Project->setText(QApplication::translate("NiirdPadClass", "New", nullptr));
        actionNew_File->setText(QApplication::translate("NiirdPadClass", "New File", nullptr));
        actionOpen->setText(QApplication::translate("NiirdPadClass", "Open...", nullptr));
        actionSave->setText(QApplication::translate("NiirdPadClass", "Save", nullptr));
        actionSave_As->setText(QApplication::translate("NiirdPadClass", "Save As...", nullptr));
        actionImportProject->setText(QApplication::translate("NiirdPadClass", "Import Project...", nullptr));
        actionExport_Project->setText(QApplication::translate("NiirdPadClass", "Export Project...", nullptr));
        actionEditReferenceWindowNew->setText(QApplication::translate("NiirdPadClass", "New", nullptr));
        actionEditReferenceWindowEdit->setText(QApplication::translate("NiirdPadClass", "Edit", nullptr));
        actionImportConsole->setText(QApplication::translate("NiirdPadClass", "Import Console", nullptr));
        actionReference->setText(QApplication::translate("NiirdPadClass", "Syntax Reference", nullptr));
        actionDatabase->setText(QApplication::translate("NiirdPadClass", "View Database", nullptr));
        actionReload->setText(QApplication::translate("NiirdPadClass", "Reload From File", nullptr));
        actionReload_2->setText(QApplication::translate("NiirdPadClass", "Reload", nullptr));
        actionEditScriptWindow_Dialogue->setText(QApplication::translate("NiirdPadClass", "Dialogue Fragment", nullptr));
        actionEditScriptWindowDialogue->setText(QApplication::translate("NiirdPadClass", "Dialogue Fragment", nullptr));
        actionEditScriptWindowOption->setText(QApplication::translate("NiirdPadClass", "Option Fragment", nullptr));
        menuFile->setTitle(QApplication::translate("NiirdPadClass", "File", nullptr));
        menuDebug->setTitle(QApplication::translate("NiirdPadClass", "Debug", nullptr));
        menuEdit_Reference_Window->setTitle(QApplication::translate("NiirdPadClass", "Edit Reference Window", nullptr));
        menuEditScriptWindow->setTitle(QApplication::translate("NiirdPadClass", "Edit Script Window", nullptr));
        menuTUScript->setTitle(QApplication::translate("NiirdPadClass", "TUScript", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NiirdPadClass: public Ui_NiirdPadClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NIIRDPAD_H
