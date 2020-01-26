#pragma once

#include <QDialog>
#include "ui_QCharacterSelectionWindow.h"

class QCharacterSelectionWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::QCharacterSelectionWindow ui;

public:
	QCharacterSelectionWindow(QWidget *parent = Q_NULLPTR);
	~QCharacterSelectionWindow();

	void UpdateChecks();
};
