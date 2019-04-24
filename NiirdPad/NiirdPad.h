#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NiirdPad.h"

class NiirdPad : public QMainWindow
{
	Q_OBJECT

private:
	Ui::NiirdPadClass ui;

	void Import();

public:
	NiirdPad(QWidget *parent = Q_NULLPTR);
};
