#pragma once

#include <QDialog>
#include "ui_QImportConsole.h"

class QImportConsole : public QDialog
{
	Q_OBJECT

public:
	QImportConsole(QWidget *parent = Q_NULLPTR);
	~QImportConsole();

private:
	Ui::QImportConsole ui;
};
