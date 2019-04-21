#include "QImportConsole.h"

#include "format.h"

QImportConsole::QImportConsole(const std::string &RootDirectory, QWidget *parent)
	: QImportConsole(parent)
{
	AddInfo(fmt::format("Attempting to import directory '{0}'...", RootDirectory));
}

QImportConsole::QImportConsole(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags(this->windowFlags() ^ Qt::WindowCloseButtonHint);

	connect(ui.btnClose, &QPushButton::clicked, this, &QDialog::accept);

	AddInfo("Just a test...");
	AddError("Error! You're a brainlet");
}

QImportConsole::~QImportConsole()
{

}

void QImportConsole::AddInfo(const std::string &Text)
{
	ui.txtConsole->insertPlainText(fmt::format("[INFO] {0}\n", Text).c_str());
}

void QImportConsole::AddError(const std::string &Text)
{
	ui.txtConsole->insertHtml(fmt::format("<p style=\"color:red\">[ERROR] {0}</p>\n", Text).c_str());
}

void QImportConsole::Import(const std::string &RootDirectory)
{

}