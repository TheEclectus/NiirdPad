/********************************************************************************
** Form generated from reading UI file 'QImportConsole.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QIMPORTCONSOLE_H
#define UI_QIMPORTCONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QImportConsole
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *txtConsole;
    QProgressBar *prgsbarSteps;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnClose;

    void setupUi(QDialog *QImportConsole)
    {
        if (QImportConsole->objectName().isEmpty())
            QImportConsole->setObjectName(QString::fromUtf8("QImportConsole"));
        QImportConsole->setWindowModality(Qt::ApplicationModal);
        QImportConsole->resize(414, 297);
        verticalLayout = new QVBoxLayout(QImportConsole);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        txtConsole = new QTextEdit(QImportConsole);
        txtConsole->setObjectName(QString::fromUtf8("txtConsole"));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(8);
        txtConsole->setFont(font);
        txtConsole->setReadOnly(true);
        txtConsole->setCursorWidth(0);
        txtConsole->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(txtConsole);

        prgsbarSteps = new QProgressBar(QImportConsole);
        prgsbarSteps->setObjectName(QString::fromUtf8("prgsbarSteps"));
        prgsbarSteps->setMaximum(0);
        prgsbarSteps->setValue(0);
        prgsbarSteps->setTextVisible(true);
        prgsbarSteps->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(prgsbarSteps);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnClose = new QPushButton(QImportConsole);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        horizontalLayout->addWidget(btnClose);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QImportConsole);

        QMetaObject::connectSlotsByName(QImportConsole);
    } // setupUi

    void retranslateUi(QDialog *QImportConsole)
    {
        QImportConsole->setWindowTitle(QApplication::translate("QImportConsole", "Import Console", nullptr));
        prgsbarSteps->setFormat(QApplication::translate("QImportConsole", "%v/%m", nullptr));
        btnClose->setText(QApplication::translate("QImportConsole", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QImportConsole: public Ui_QImportConsole {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QIMPORTCONSOLE_H
