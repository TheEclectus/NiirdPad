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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
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
    QAction *actionDump_Reference_Database;
    QAction *actionExperimental_TUScript_Parser;
    QAction *actionCharacter_Selection_Window;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnAddCharacter;
    QPushButton *btnDeleteCharacter;
    QPushButton *btnRenameCharacter;
    QComboBox *cmbCharacter;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnAddDiagFile;
    QPushButton *btnDeleteDiagFile;
    QPushButton *btnRenameDiagFile;
    QComboBox *cmbDiag;
    QFrame *line;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *cmbJump;
    QPushButton *btnJump;
    QSpacerItem *verticalSpacer;
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
        NiirdPadClass->resize(1098, 644);
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
        actionImportProject->setEnabled(true);
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
        actionDump_Reference_Database = new QAction(NiirdPadClass);
        actionDump_Reference_Database->setObjectName(QString::fromUtf8("actionDump_Reference_Database"));
        actionExperimental_TUScript_Parser = new QAction(NiirdPadClass);
        actionExperimental_TUScript_Parser->setObjectName(QString::fromUtf8("actionExperimental_TUScript_Parser"));
        actionCharacter_Selection_Window = new QAction(NiirdPadClass);
        actionCharacter_Selection_Window->setObjectName(QString::fromUtf8("actionCharacter_Selection_Window"));
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
        frame->setMinimumSize(QSize(185, 0));
        frame->setMaximumSize(QSize(175, 16777215));
        frame->setSizeIncrement(QSize(0, 0));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Plain);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout_3->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnAddCharacter = new QPushButton(frame);
        btnAddCharacter->setObjectName(QString::fromUtf8("btnAddCharacter"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnAddCharacter->sizePolicy().hasHeightForWidth());
        btnAddCharacter->setSizePolicy(sizePolicy1);
        btnAddCharacter->setMinimumSize(QSize(23, 0));
        btnAddCharacter->setMaximumSize(QSize(23, 16777215));

        horizontalLayout_4->addWidget(btnAddCharacter);

        btnDeleteCharacter = new QPushButton(frame);
        btnDeleteCharacter->setObjectName(QString::fromUtf8("btnDeleteCharacter"));
        btnDeleteCharacter->setMinimumSize(QSize(23, 0));
        btnDeleteCharacter->setMaximumSize(QSize(23, 16777215));

        horizontalLayout_4->addWidget(btnDeleteCharacter);

        btnRenameCharacter = new QPushButton(frame);
        btnRenameCharacter->setObjectName(QString::fromUtf8("btnRenameCharacter"));
        btnRenameCharacter->setMinimumSize(QSize(45, 0));

        horizontalLayout_4->addWidget(btnRenameCharacter);


        horizontalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout->addLayout(horizontalLayout_3);

        cmbCharacter = new QComboBox(frame);
        cmbCharacter->setObjectName(QString::fromUtf8("cmbCharacter"));

        verticalLayout->addWidget(cmbCharacter);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        horizontalLayout_5->addWidget(label_2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        btnAddDiagFile = new QPushButton(frame);
        btnAddDiagFile->setObjectName(QString::fromUtf8("btnAddDiagFile"));
        sizePolicy1.setHeightForWidth(btnAddDiagFile->sizePolicy().hasHeightForWidth());
        btnAddDiagFile->setSizePolicy(sizePolicy1);
        btnAddDiagFile->setMinimumSize(QSize(23, 0));
        btnAddDiagFile->setMaximumSize(QSize(23, 16777215));

        horizontalLayout_7->addWidget(btnAddDiagFile);

        btnDeleteDiagFile = new QPushButton(frame);
        btnDeleteDiagFile->setObjectName(QString::fromUtf8("btnDeleteDiagFile"));
        btnDeleteDiagFile->setMinimumSize(QSize(23, 0));
        btnDeleteDiagFile->setMaximumSize(QSize(23, 16777215));

        horizontalLayout_7->addWidget(btnDeleteDiagFile);

        btnRenameDiagFile = new QPushButton(frame);
        btnRenameDiagFile->setObjectName(QString::fromUtf8("btnRenameDiagFile"));
        btnRenameDiagFile->setMinimumSize(QSize(45, 0));

        horizontalLayout_7->addWidget(btnRenameDiagFile);


        horizontalLayout_5->addLayout(horizontalLayout_7);


        verticalLayout->addLayout(horizontalLayout_5);

        cmbDiag = new QComboBox(frame);
        cmbDiag->setObjectName(QString::fromUtf8("cmbDiag"));

        verticalLayout->addWidget(cmbDiag);

        line = new QFrame(frame);
        line->setObjectName(QString::fromUtf8("line"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy2);
        line->setMinimumSize(QSize(0, 1));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        cmbJump = new QComboBox(frame);
        cmbJump->setObjectName(QString::fromUtf8("cmbJump"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(cmbJump->sizePolicy().hasHeightForWidth());
        cmbJump->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(cmbJump);

        btnJump = new QPushButton(frame);
        btnJump->setObjectName(QString::fromUtf8("btnJump"));
        sizePolicy1.setHeightForWidth(btnJump->sizePolicy().hasHeightForWidth());
        btnJump->setSizePolicy(sizePolicy1);
        btnJump->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(btnJump);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(frame);

        widget = new QNodeView(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(20);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy4);

        horizontalLayout->addWidget(widget);

        NiirdPadClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(NiirdPadClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1098, 21));
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
        menuDebug->addAction(actionDump_Reference_Database);
        menuDebug->addAction(actionExperimental_TUScript_Parser);
        menuDebug->addAction(actionCharacter_Selection_Window);
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
        actionExport_Project->setText(QApplication::translate("NiirdPadClass", "x Export Project...", nullptr));
#ifndef QT_NO_TOOLTIP
        actionExport_Project->setToolTip(QApplication::translate("NiirdPadClass", "x Export Project", nullptr));
#endif // QT_NO_TOOLTIP
        actionEditReferenceWindowNew->setText(QApplication::translate("NiirdPadClass", "New", nullptr));
        actionEditReferenceWindowEdit->setText(QApplication::translate("NiirdPadClass", "Edit", nullptr));
        actionImportConsole->setText(QApplication::translate("NiirdPadClass", "Import Console", nullptr));
        actionReference->setText(QApplication::translate("NiirdPadClass", "Syntax Reference", nullptr));
        actionDatabase->setText(QApplication::translate("NiirdPadClass", "x View Database", nullptr));
        actionReload->setText(QApplication::translate("NiirdPadClass", "x Reload From File", nullptr));
        actionReload_2->setText(QApplication::translate("NiirdPadClass", "x Reload", nullptr));
        actionEditScriptWindow_Dialogue->setText(QApplication::translate("NiirdPadClass", "Dialogue Fragment", nullptr));
        actionEditScriptWindowDialogue->setText(QApplication::translate("NiirdPadClass", "Dialogue Fragment", nullptr));
        actionEditScriptWindowOption->setText(QApplication::translate("NiirdPadClass", "Option Fragment", nullptr));
        actionDump_Reference_Database->setText(QApplication::translate("NiirdPadClass", "Dump Reference Database For Current File", nullptr));
        actionExperimental_TUScript_Parser->setText(QApplication::translate("NiirdPadClass", "Experimental TUScript Parser", nullptr));
        actionCharacter_Selection_Window->setText(QApplication::translate("NiirdPadClass", "Character Selection Window", nullptr));
        label->setText(QApplication::translate("NiirdPadClass", "Character", nullptr));
#ifndef QT_NO_TOOLTIP
        btnAddCharacter->setToolTip(QApplication::translate("NiirdPadClass", "Create New Character", nullptr));
#endif // QT_NO_TOOLTIP
        btnAddCharacter->setText(QApplication::translate("NiirdPadClass", "+", nullptr));
#ifndef QT_NO_TOOLTIP
        btnDeleteCharacter->setToolTip(QApplication::translate("NiirdPadClass", "Delete Character", nullptr));
#endif // QT_NO_TOOLTIP
        btnDeleteCharacter->setText(QApplication::translate("NiirdPadClass", "-", nullptr));
#ifndef QT_NO_TOOLTIP
        btnRenameCharacter->setToolTip(QApplication::translate("NiirdPadClass", "Rename Character", nullptr));
#endif // QT_NO_TOOLTIP
        btnRenameCharacter->setText(QApplication::translate("NiirdPadClass", "Ren", nullptr));
        label_2->setText(QApplication::translate("NiirdPadClass", "Diag File", nullptr));
#ifndef QT_NO_TOOLTIP
        btnAddDiagFile->setToolTip(QApplication::translate("NiirdPadClass", "Create New Character", nullptr));
#endif // QT_NO_TOOLTIP
        btnAddDiagFile->setText(QApplication::translate("NiirdPadClass", "+", nullptr));
#ifndef QT_NO_TOOLTIP
        btnDeleteDiagFile->setToolTip(QApplication::translate("NiirdPadClass", "Delete Character", nullptr));
#endif // QT_NO_TOOLTIP
        btnDeleteDiagFile->setText(QApplication::translate("NiirdPadClass", "-", nullptr));
#ifndef QT_NO_TOOLTIP
        btnRenameDiagFile->setToolTip(QApplication::translate("NiirdPadClass", "Rename Character", nullptr));
#endif // QT_NO_TOOLTIP
        btnRenameDiagFile->setText(QApplication::translate("NiirdPadClass", "Ren", nullptr));
        label_3->setText(QApplication::translate("NiirdPadClass", "Jump to Index", nullptr));
        btnJump->setText(QApplication::translate("NiirdPadClass", "Jump", nullptr));
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
