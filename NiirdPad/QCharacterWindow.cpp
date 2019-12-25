#include "QCharacterWindow.h"

#include <QEvent>
#include <QMessageBox>
#include <QTextBlock>

#include "Character.h"
#include "Project.h"

void QCharacterWindow::MakeError(const std::string &Message)
{
	ui.lblInfo->setText(Message.c_str());
	QPalette Pal = ui.lblInfo->palette();
	Pal.setColor(ui.lblInfo->foregroundRole(), Qt::red);
	ui.lblInfo->setPalette(Pal);

	ui.btnAccept->setDisabled(true);
}

void QCharacterWindow::MakeClean()
{
	ui.lblInfo->setText("No errors found.");
	QPalette Pal = ui.lblInfo->palette();
	Pal.setColor(ui.lblInfo->foregroundRole(), Qt::darkGreen);
	ui.lblInfo->setPalette(Pal);

	ui.btnAccept->setDisabled(false);
}

QCharacterWindow::QCharacterWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.txtCharacterName, &QLineEdit::textChanged, [this]() {
		if (this->isVisible())
		{
			this->Validate();
		}
	});

	connect(this, &QDialog::accepted, this, &QCharacterWindow::FormAccepted);
	connect(ui.btnAccept, &QPushButton::clicked, [this]() {
		accept();
	});
	connect(ui.btnCancel, &QPushButton::clicked, this, &QCharacterWindow::Close);
}

QCharacterWindow::~QCharacterWindow()
{

}

void QCharacterWindow::closeEvent(QCloseEvent *event)
{
	event->ignore();
	Close();
}

void QCharacterWindow::ResetForm()
{
	_editCharacter = nullptr;
	_addCharacter = nullptr;

	ui.txtCharacterName->clear();
}

void QCharacterWindow::FormAccepted()
{
	if (_editCharacter)
	{
		_addCharacter->RenameCharacter(_editCharacter, ui.txtCharacterName->text().toStdString());
		emit NameEdited(ui.txtCharacterName->text().toStdString());
	}
	else if (_addCharacter)
	{
		auto NewChar = _addCharacter->NewCharacter(ui.txtCharacterName->text().toStdString());
		NewChar->NewDialogueFile("diag.txt");

		emit NameAdded(ui.txtCharacterName->text().toStdString());
	}

	ResetForm();
	//accept();
}

void QCharacterWindow::Close()
{
	ResetForm();
	reject();
}

void QCharacterWindow::Validate()
{
	std::string CharName = ui.txtCharacterName->text().toStdString();
	if (CharName.length() == 0)
	{
		MakeError("Character name cannot be empty.");
		return;
	}

	if (CharName.find_first_not_of("abcdefghijklmnopqrstuvwxyz1234567890_") != CharName.npos)
	{
		MakeError("Character name contains illegal characters.");
		return;
	}

	if (_editCharacter != nullptr)
	{
		auto &Chars = _addCharacter->Characters();
		auto Res = Chars.find(CharName);
		if (Res != Chars.end() && (*Res).second != _editCharacter)
		{
			MakeError("Character name already exists.");
			return;
		}
	}
	else if (_addCharacter != nullptr)
	{
		auto &Chars = _addCharacter->Characters();
		if (Chars.find(CharName) != Chars.end())
		{
			MakeError("Character name already exists.");
			return;
		}
	}

	MakeClean();
}

void QCharacterWindow::NewCharacter(Project *Proj)
{
	_addCharacter = Proj;

	this->setWindowTitle("New Character");
	this->Validate();
	this->show();
}

void QCharacterWindow::EditCharacter(Project *Proj, Character *Char)
{
	_addCharacter = Proj;
	_editCharacter = Char;

	ui.txtCharacterName->setText(Char->GetName().c_str());

	this->setWindowTitle("Edit Character");
	this->Validate();
	this->show();
}