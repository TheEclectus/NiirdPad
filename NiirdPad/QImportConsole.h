#pragma once

#include <QDialog>
#include "ui_QImportConsole.h"

class QImportConsole : public QDialog
{
	Q_OBJECT

private:
	Ui::QImportConsole ui;

public:
	QImportConsole(const std::string &RootDirectory, QWidget *parent = Q_NULLPTR);
	QImportConsole(QWidget *parent = Q_NULLPTR);
	~QImportConsole();

	void AddInfo(const std::string &Text);
	void AddError(const std::string &Text);

	static void Import(const std::string &RootDirectory);
};
