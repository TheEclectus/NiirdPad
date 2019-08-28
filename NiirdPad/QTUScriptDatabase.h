#pragma once

#include <QDialog>
#include "ui_QTUScriptDatabase.h"

class QTUScriptDatabase : public QDialog
{
	Q_OBJECT

private:
	Ui::QTUScriptDatabase ui;

public:
	QTUScriptDatabase(QWidget *parent = Q_NULLPTR);
	~QTUScriptDatabase();
};
