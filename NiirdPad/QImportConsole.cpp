#include "QImportConsole.h"

QImportConsole::QImportConsole(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags(this->windowFlags() ^ Qt::WindowCloseButtonHint);

	connect(ui.btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

QImportConsole::~QImportConsole()
{
}
