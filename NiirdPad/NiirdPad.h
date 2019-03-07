#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NiirdPad.h"

class NiirdPad : public QMainWindow
{
	Q_OBJECT

public:
	NiirdPad(QWidget *parent = Q_NULLPTR);

private:
	Ui::NiirdPadClass ui;
};
