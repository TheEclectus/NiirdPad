/********************************************************************************
** Form generated from reading UI file 'QCharacterSelectionWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCHARACTERSELECTIONWINDOW_H
#define UI_QCHARACTERSELECTIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QCharacterSelectionWindow
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lblSelection;
    QSpacerItem *horizontalSpacer;
    QTreeWidget *treeSelection;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnSelection_SelectAll;
    QPushButton *btnSelection_DeselectAll;
    QFrame *line;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lblExisting;
    QSpacerItem *horizontalSpacer_4;
    QTreeWidget *treeExisting;
    QCheckBox *chkClearExisting;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btnAccept;
    QPushButton *btnCancel;

    void setupUi(QDialog *QCharacterSelectionWindow)
    {
        if (QCharacterSelectionWindow->objectName().isEmpty())
            QCharacterSelectionWindow->setObjectName(QString::fromUtf8("QCharacterSelectionWindow"));
        QCharacterSelectionWindow->resize(749, 588);
        verticalLayout = new QVBoxLayout(QCharacterSelectionWindow);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        lblSelection = new QLabel(QCharacterSelectionWindow);
        lblSelection->setObjectName(QString::fromUtf8("lblSelection"));

        horizontalLayout_4->addWidget(lblSelection);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_4);

        treeSelection = new QTreeWidget(QCharacterSelectionWindow);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeSelection->setHeaderItem(__qtreewidgetitem);
        treeSelection->setObjectName(QString::fromUtf8("treeSelection"));
        treeSelection->setRootIsDecorated(true);
        treeSelection->setUniformRowHeights(false);
        treeSelection->setWordWrap(true);
        treeSelection->setHeaderHidden(false);

        verticalLayout_2->addWidget(treeSelection);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btnSelection_SelectAll = new QPushButton(QCharacterSelectionWindow);
        btnSelection_SelectAll->setObjectName(QString::fromUtf8("btnSelection_SelectAll"));

        horizontalLayout_2->addWidget(btnSelection_SelectAll);

        btnSelection_DeselectAll = new QPushButton(QCharacterSelectionWindow);
        btnSelection_DeselectAll->setObjectName(QString::fromUtf8("btnSelection_DeselectAll"));

        horizontalLayout_2->addWidget(btnSelection_DeselectAll);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout->addLayout(verticalLayout_2);

        line = new QFrame(QCharacterSelectionWindow);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        lblExisting = new QLabel(QCharacterSelectionWindow);
        lblExisting->setObjectName(QString::fromUtf8("lblExisting"));

        horizontalLayout_5->addWidget(lblExisting);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_5);

        treeExisting = new QTreeWidget(QCharacterSelectionWindow);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        treeExisting->setHeaderItem(__qtreewidgetitem1);
        treeExisting->setObjectName(QString::fromUtf8("treeExisting"));
        treeExisting->setRootIsDecorated(true);
        treeExisting->setUniformRowHeights(false);
        treeExisting->setWordWrap(true);
        treeExisting->setHeaderHidden(false);

        verticalLayout_3->addWidget(treeExisting);

        chkClearExisting = new QCheckBox(QCharacterSelectionWindow);
        chkClearExisting->setObjectName(QString::fromUtf8("chkClearExisting"));

        verticalLayout_3->addWidget(chkClearExisting);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

        verticalLayout_3->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        btnAccept = new QPushButton(QCharacterSelectionWindow);
        btnAccept->setObjectName(QString::fromUtf8("btnAccept"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnAccept->sizePolicy().hasHeightForWidth());
        btnAccept->setSizePolicy(sizePolicy);
        btnAccept->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(btnAccept);

        btnCancel = new QPushButton(QCharacterSelectionWindow);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        sizePolicy.setHeightForWidth(btnCancel->sizePolicy().hasHeightForWidth());
        btnCancel->setSizePolicy(sizePolicy);
        btnCancel->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout_6);


        retranslateUi(QCharacterSelectionWindow);

        QMetaObject::connectSlotsByName(QCharacterSelectionWindow);
    } // setupUi

    void retranslateUi(QDialog *QCharacterSelectionWindow)
    {
        QCharacterSelectionWindow->setWindowTitle(QApplication::translate("QCharacterSelectionWindow", "QImportWindow", nullptr));
        lblSelection->setText(QApplication::translate("QCharacterSelectionWindow", "Characters to Import", nullptr));
        btnSelection_SelectAll->setText(QApplication::translate("QCharacterSelectionWindow", "Select All", nullptr));
        btnSelection_DeselectAll->setText(QApplication::translate("QCharacterSelectionWindow", "Deselect All", nullptr));
        lblExisting->setText(QApplication::translate("QCharacterSelectionWindow", "Existing Characters", nullptr));
#ifndef QT_NO_TOOLTIP
        chkClearExisting->setToolTip(QApplication::translate("QCharacterSelectionWindow", "Should existing characters be deleted before importing the new ones?", nullptr));
#endif // QT_NO_TOOLTIP
        chkClearExisting->setText(QApplication::translate("QCharacterSelectionWindow", "Clear Existing Characters", nullptr));
        btnAccept->setText(QApplication::translate("QCharacterSelectionWindow", "Accept", nullptr));
        btnCancel->setText(QApplication::translate("QCharacterSelectionWindow", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QCharacterSelectionWindow: public Ui_QCharacterSelectionWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCHARACTERSELECTIONWINDOW_H
