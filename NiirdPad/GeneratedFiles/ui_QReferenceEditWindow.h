/********************************************************************************
** Form generated from reading UI file 'QReferenceEditWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QREFERENCEEDITWINDOW_H
#define UI_QREFERENCEEDITWINDOW_H

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

class Ui_QReferenceEditWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *txtReferenceEdit;
    QLabel *lblInfo;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnAccept;
    QPushButton *btnCancel;

    void setupUi(QDialog *QReferenceEditWindow)
    {
        if (QReferenceEditWindow->objectName().isEmpty())
            QReferenceEditWindow->setObjectName(QString::fromUtf8("QReferenceEditWindow"));
        QReferenceEditWindow->setWindowModality(Qt::ApplicationModal);
        QReferenceEditWindow->resize(282, 114);
        verticalLayout = new QVBoxLayout(QReferenceEditWindow);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        txtReferenceEdit = new QLineEdit(QReferenceEditWindow);
        txtReferenceEdit->setObjectName(QString::fromUtf8("txtReferenceEdit"));

        verticalLayout->addWidget(txtReferenceEdit);

        lblInfo = new QLabel(QReferenceEditWindow);
        lblInfo->setObjectName(QString::fromUtf8("lblInfo"));
        lblInfo->setWordWrap(true);

        verticalLayout->addWidget(lblInfo);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnAccept = new QPushButton(QReferenceEditWindow);
        btnAccept->setObjectName(QString::fromUtf8("btnAccept"));

        horizontalLayout->addWidget(btnAccept);

        btnCancel = new QPushButton(QReferenceEditWindow);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QReferenceEditWindow);

        QMetaObject::connectSlotsByName(QReferenceEditWindow);
    } // setupUi

    void retranslateUi(QDialog *QReferenceEditWindow)
    {
        QReferenceEditWindow->setWindowTitle(QApplication::translate("QReferenceEditWindow", "Edit Reference", nullptr));
        lblInfo->setText(QApplication::translate("QReferenceEditWindow", "TextLabel but now I'm gonna make it dummy thicc to see how it handles resizing past its predefined bounds.", nullptr));
        btnAccept->setText(QApplication::translate("QReferenceEditWindow", "Accept", nullptr));
        btnCancel->setText(QApplication::translate("QReferenceEditWindow", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QReferenceEditWindow: public Ui_QReferenceEditWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QREFERENCEEDITWINDOW_H
