/********************************************************************************
** Form generated from reading UI file 'QScriptEditWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QSCRIPTEDITWINDOW_H
#define UI_QSCRIPTEDITWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QScriptEditWindow
{
public:
    QVBoxLayout *verticalLayout_5;
    QSplitter *splitter;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_3;
    QTextEdit *txtVisibility;
    QLabel *lblVisErrors;
    QFrame *line_2;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QFrame *line;
    QTextEdit *txtScripts;
    QLabel *lblScriptErrors;
    QFrame *line_3;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_4;
    QFrame *line_4;
    QTextEdit *txtText;
    QLabel *lblTxtErrors;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnAccept;
    QPushButton *btnCancel;

    void setupUi(QDialog *QScriptEditWindow)
    {
        if (QScriptEditWindow->objectName().isEmpty())
            QScriptEditWindow->setObjectName(QString::fromUtf8("QScriptEditWindow"));
        QScriptEditWindow->setWindowModality(Qt::ApplicationModal);
        QScriptEditWindow->resize(294, 419);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QScriptEditWindow->sizePolicy().hasHeightForWidth());
        QScriptEditWindow->setSizePolicy(sizePolicy);
        QScriptEditWindow->setMinimumSize(QSize(200, 305));
        QScriptEditWindow->setModal(true);
        verticalLayout_5 = new QVBoxLayout(QScriptEditWindow);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        splitter = new QSplitter(QScriptEditWindow);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setStyleSheet(QString::fromUtf8("QSplitter::handle {background: rgb(177, 177, 177);}"));
        splitter->setOrientation(Qt::Vertical);
        splitter->setOpaqueResize(true);
        splitter->setHandleWidth(3);
        splitter->setChildrenCollapsible(false);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        txtVisibility = new QTextEdit(verticalLayoutWidget);
        txtVisibility->setObjectName(QString::fromUtf8("txtVisibility"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(txtVisibility->sizePolicy().hasHeightForWidth());
        txtVisibility->setSizePolicy(sizePolicy1);
        txtVisibility->setMinimumSize(QSize(0, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(8);
        txtVisibility->setFont(font);

        verticalLayout_3->addWidget(txtVisibility);

        lblVisErrors = new QLabel(verticalLayoutWidget);
        lblVisErrors->setObjectName(QString::fromUtf8("lblVisErrors"));

        verticalLayout_3->addWidget(lblVisErrors);

        line_2 = new QFrame(verticalLayoutWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setLineWidth(0);
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_2);

        splitter->addWidget(verticalLayoutWidget);
        verticalLayoutWidget_2 = new QWidget(splitter);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        line = new QFrame(verticalLayoutWidget_2);
        line->setObjectName(QString::fromUtf8("line"));
        line->setLineWidth(0);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        txtScripts = new QTextEdit(verticalLayoutWidget_2);
        txtScripts->setObjectName(QString::fromUtf8("txtScripts"));
        sizePolicy1.setHeightForWidth(txtScripts->sizePolicy().hasHeightForWidth());
        txtScripts->setSizePolicy(sizePolicy1);
        txtScripts->setMinimumSize(QSize(0, 50));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Consolas"));
        txtScripts->setFont(font1);
        txtScripts->setAcceptRichText(false);

        verticalLayout_2->addWidget(txtScripts);

        lblScriptErrors = new QLabel(verticalLayoutWidget_2);
        lblScriptErrors->setObjectName(QString::fromUtf8("lblScriptErrors"));

        verticalLayout_2->addWidget(lblScriptErrors);

        line_3 = new QFrame(verticalLayoutWidget_2);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setLineWidth(0);
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_3);

        splitter->addWidget(verticalLayoutWidget_2);
        verticalLayoutWidget_3 = new QWidget(splitter);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_4->setSpacing(2);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        line_4 = new QFrame(verticalLayoutWidget_3);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setLineWidth(0);
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_4);

        txtText = new QTextEdit(verticalLayoutWidget_3);
        txtText->setObjectName(QString::fromUtf8("txtText"));
        sizePolicy1.setHeightForWidth(txtText->sizePolicy().hasHeightForWidth());
        txtText->setSizePolicy(sizePolicy1);
        txtText->setMinimumSize(QSize(0, 50));
        txtText->setFont(font1);
        txtText->setAcceptRichText(false);

        verticalLayout_4->addWidget(txtText);

        lblTxtErrors = new QLabel(verticalLayoutWidget_3);
        lblTxtErrors->setObjectName(QString::fromUtf8("lblTxtErrors"));

        verticalLayout_4->addWidget(lblTxtErrors);

        splitter->addWidget(verticalLayoutWidget_3);

        verticalLayout_5->addWidget(splitter);

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

        btnAccept = new QPushButton(QScriptEditWindow);
        btnAccept->setObjectName(QString::fromUtf8("btnAccept"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btnAccept->sizePolicy().hasHeightForWidth());
        btnAccept->setSizePolicy(sizePolicy2);
        btnAccept->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(btnAccept);

        btnCancel = new QPushButton(QScriptEditWindow);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        sizePolicy2.setHeightForWidth(btnCancel->sizePolicy().hasHeightForWidth());
        btnCancel->setSizePolicy(sizePolicy2);
        btnCancel->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_5->addLayout(verticalLayout);


        retranslateUi(QScriptEditWindow);

        QMetaObject::connectSlotsByName(QScriptEditWindow);
    } // setupUi

    void retranslateUi(QDialog *QScriptEditWindow)
    {
        QScriptEditWindow->setWindowTitle(QApplication::translate("QScriptEditWindow", "Edit Fragment", nullptr));
        txtVisibility->setPlaceholderText(QApplication::translate("QScriptEditWindow", "Visibility Conditions", nullptr));
        lblVisErrors->setText(QApplication::translate("QScriptEditWindow", "<html><head/><body><p>No errors found.</p></body></html>", nullptr));
        txtScripts->setPlaceholderText(QApplication::translate("QScriptEditWindow", "TUScript Functions", nullptr));
        lblScriptErrors->setText(QApplication::translate("QScriptEditWindow", "<html><head/><body><p>No errors found.</p></body></html>", nullptr));
        txtText->setPlaceholderText(QApplication::translate("QScriptEditWindow", "Option or Dialogue", nullptr));
        lblTxtErrors->setText(QApplication::translate("QScriptEditWindow", "<html><head/><body><p>No errors found.</p></body></html>", nullptr));
        btnAccept->setText(QApplication::translate("QScriptEditWindow", "Accept", nullptr));
        btnCancel->setText(QApplication::translate("QScriptEditWindow", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QScriptEditWindow: public Ui_QScriptEditWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSCRIPTEDITWINDOW_H
