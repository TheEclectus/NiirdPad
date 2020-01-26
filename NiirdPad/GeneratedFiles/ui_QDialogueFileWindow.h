/********************************************************************************
** Form generated from reading UI file 'QDialogueFileWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDIALOGUEFILEWINDOW_H
#define UI_QDIALOGUEFILEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QDialogueFileWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *txtDialogueFileName;
    QLabel *lblFileName;
    QLabel *lblInfo;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnAccept;
    QPushButton *btnCancel;

    void setupUi(QDialog *QDialogueFileWindow)
    {
        if (QDialogueFileWindow->objectName().isEmpty())
            QDialogueFileWindow->setObjectName(QString::fromUtf8("QDialogueFileWindow"));
        QDialogueFileWindow->resize(282, 127);
        verticalLayout = new QVBoxLayout(QDialogueFileWindow);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        txtDialogueFileName = new QLineEdit(QDialogueFileWindow);
        txtDialogueFileName->setObjectName(QString::fromUtf8("txtDialogueFileName"));

        verticalLayout->addWidget(txtDialogueFileName);

        lblFileName = new QLabel(QDialogueFileWindow);
        lblFileName->setObjectName(QString::fromUtf8("lblFileName"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        lblFileName->setFont(font);

        verticalLayout->addWidget(lblFileName);

        lblInfo = new QLabel(QDialogueFileWindow);
        lblInfo->setObjectName(QString::fromUtf8("lblInfo"));

        verticalLayout->addWidget(lblInfo);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnAccept = new QPushButton(QDialogueFileWindow);
        btnAccept->setObjectName(QString::fromUtf8("btnAccept"));

        horizontalLayout->addWidget(btnAccept);

        btnCancel = new QPushButton(QDialogueFileWindow);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QDialogueFileWindow);

        QMetaObject::connectSlotsByName(QDialogueFileWindow);
    } // setupUi

    void retranslateUi(QDialog *QDialogueFileWindow)
    {
        QDialogueFileWindow->setWindowTitle(QApplication::translate("QDialogueFileWindow", "New Dialogue File", nullptr));
        lblFileName->setText(QApplication::translate("QDialogueFileWindow", "Final filename label", nullptr));
        lblInfo->setText(QApplication::translate("QDialogueFileWindow", "Error label", nullptr));
        btnAccept->setText(QApplication::translate("QDialogueFileWindow", "Accept", nullptr));
        btnCancel->setText(QApplication::translate("QDialogueFileWindow", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QDialogueFileWindow: public Ui_QDialogueFileWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDIALOGUEFILEWINDOW_H
