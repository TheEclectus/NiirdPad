/********************************************************************************
** Form generated from reading UI file 'QTUScriptDatabase.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTUSCRIPTDATABASE_H
#define UI_QTUSCRIPTDATABASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QTUScriptDatabase
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout;
    QListView *listView;
    QTextEdit *textEdit;
    QWidget *tab_2;

    void setupUi(QDialog *QTUScriptDatabase)
    {
        if (QTUScriptDatabase->objectName().isEmpty())
            QTUScriptDatabase->setObjectName(QString::fromUtf8("QTUScriptDatabase"));
        QTUScriptDatabase->resize(895, 649);
        verticalLayout = new QVBoxLayout(QTUScriptDatabase);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(QTUScriptDatabase);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout = new QHBoxLayout(tab);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listView = new QListView(tab);
        listView->setObjectName(QString::fromUtf8("listView"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        listView->setFont(font);

        horizontalLayout->addWidget(listView);

        textEdit = new QTextEdit(tab);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QPalette palette;
        QBrush brush(QColor(240, 240, 240, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        textEdit->setPalette(palette);
        textEdit->setFont(font);
        textEdit->setAutoFillBackground(false);

        horizontalLayout->addWidget(textEdit);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(QTUScriptDatabase);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QTUScriptDatabase);
    } // setupUi

    void retranslateUi(QDialog *QTUScriptDatabase)
    {
        QTUScriptDatabase->setWindowTitle(QApplication::translate("QTUScriptDatabase", "QTUScriptDatabase", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("QTUScriptDatabase", "Functions", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("QTUScriptDatabase", "Visibility", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QTUScriptDatabase: public Ui_QTUScriptDatabase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTUSCRIPTDATABASE_H
