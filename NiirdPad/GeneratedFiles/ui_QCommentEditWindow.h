/********************************************************************************
** Form generated from reading UI file 'QCommentEditWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCOMMENTEDITWINDOW_H
#define UI_QCOMMENTEDITWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QCommentEditWindow
{
public:
    QVBoxLayout *verticalLayout_2;
    QTextEdit *txtComment;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnAccept;
    QPushButton *btnCancel;

    void setupUi(QDialog *QCommentEditWindow)
    {
        if (QCommentEditWindow->objectName().isEmpty())
            QCommentEditWindow->setObjectName(QString::fromUtf8("QCommentEditWindow"));
        QCommentEditWindow->resize(336, 101);
        QCommentEditWindow->setModal(true);
        verticalLayout_2 = new QVBoxLayout(QCommentEditWindow);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        txtComment = new QTextEdit(QCommentEditWindow);
        txtComment->setObjectName(QString::fromUtf8("txtComment"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(txtComment->sizePolicy().hasHeightForWidth());
        txtComment->setSizePolicy(sizePolicy);
        txtComment->setMinimumSize(QSize(0, 50));
        txtComment->setInputMethodHints(Qt::ImhNone);
        txtComment->setAcceptRichText(false);

        verticalLayout_2->addWidget(txtComment);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnAccept = new QPushButton(QCommentEditWindow);
        btnAccept->setObjectName(QString::fromUtf8("btnAccept"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnAccept->sizePolicy().hasHeightForWidth());
        btnAccept->setSizePolicy(sizePolicy1);
        btnAccept->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(btnAccept);

        btnCancel = new QPushButton(QCommentEditWindow);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        sizePolicy1.setHeightForWidth(btnCancel->sizePolicy().hasHeightForWidth());
        btnCancel->setSizePolicy(sizePolicy1);
        btnCancel->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(QCommentEditWindow);

        QMetaObject::connectSlotsByName(QCommentEditWindow);
    } // setupUi

    void retranslateUi(QDialog *QCommentEditWindow)
    {
        QCommentEditWindow->setWindowTitle(QApplication::translate("QCommentEditWindow", "QCommentEditWindow", nullptr));
        txtComment->setPlaceholderText(QApplication::translate("QCommentEditWindow", "Comment", nullptr));
        btnAccept->setText(QApplication::translate("QCommentEditWindow", "Accept", nullptr));
        btnCancel->setText(QApplication::translate("QCommentEditWindow", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QCommentEditWindow: public Ui_QCommentEditWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCOMMENTEDITWINDOW_H
