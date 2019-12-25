/********************************************************************************
** Form generated from reading UI file 'QCharacterWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCHARACTERWINDOW_H
#define UI_QCHARACTERWINDOW_H

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

class Ui_QCharacterWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *txtCharacterName;
    QLabel *lblInfo;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnAccept;
    QPushButton *btnCancel;

    void setupUi(QDialog *QCharacterWindow)
    {
        if (QCharacterWindow->objectName().isEmpty())
            QCharacterWindow->setObjectName(QString::fromUtf8("QCharacterWindow"));
        QCharacterWindow->resize(283, 101);
        verticalLayout = new QVBoxLayout(QCharacterWindow);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        txtCharacterName = new QLineEdit(QCharacterWindow);
        txtCharacterName->setObjectName(QString::fromUtf8("txtCharacterName"));

        verticalLayout->addWidget(txtCharacterName);

        lblInfo = new QLabel(QCharacterWindow);
        lblInfo->setObjectName(QString::fromUtf8("lblInfo"));

        verticalLayout->addWidget(lblInfo);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnAccept = new QPushButton(QCharacterWindow);
        btnAccept->setObjectName(QString::fromUtf8("btnAccept"));

        horizontalLayout->addWidget(btnAccept);

        btnCancel = new QPushButton(QCharacterWindow);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QCharacterWindow);

        QMetaObject::connectSlotsByName(QCharacterWindow);
    } // setupUi

    void retranslateUi(QDialog *QCharacterWindow)
    {
        QCharacterWindow->setWindowTitle(QApplication::translate("QCharacterWindow", "New Character", nullptr));
        lblInfo->setText(QApplication::translate("QCharacterWindow", "Error label", nullptr));
        btnAccept->setText(QApplication::translate("QCharacterWindow", "Accept", nullptr));
        btnCancel->setText(QApplication::translate("QCharacterWindow", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QCharacterWindow: public Ui_QCharacterWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCHARACTERWINDOW_H
