#include "QDialogueFileWindow.h"

#include <QEvent>
#include <QMessageBox>
#include <QTextBlock>

#include "Character.h"
#include "DialogueFile.h"

void QDialogueFileWindow::MakeError(const std::string &Message)
{
	ui.lblInfo->setText(Message.c_str());
	QPalette Pal = ui.lblInfo->palette();
	Pal.setColor(ui.lblInfo->foregroundRole(), Qt::red);
	ui.lblInfo->setPalette(Pal);

	ui.btnAccept->setDisabled(true);
}

void QDialogueFileWindow::MakeClean()
{
	ui.lblInfo->setText("No errors found.");
	QPalette Pal = ui.lblInfo->palette();
	Pal.setColor(ui.lblInfo->foregroundRole(), Qt::darkGreen);
	ui.lblInfo->setPalette(Pal);

	ui.btnAccept->setDisabled(false);
}

QDialogueFileWindow::QDialogueFileWindow(QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.txtDialogueFileName, &QLineEdit::textChanged, [this]() {
		if (this->isVisible())
		{
			this->Validate();
		}
	});

	connect(this, &QDialog::accepted, this, &QDialogueFileWindow::FormAccepted);
	connect(ui.btnAccept, &QPushButton::clicked, [this]() {
		accept();
	});
	connect(ui.btnCancel, &QPushButton::clicked, this, &QDialogueFileWindow::Close);
}

QDialogueFileWindow::~QDialogueFileWindow()
{

}

void QDialogueFileWindow::closeEvent(QCloseEvent *event)
{
	event->ignore();
	Close();
}

void QDialogueFileWindow::ResetForm()
{
	_editDialogueFile = nullptr;
	_addDialogueFile = nullptr;

	ui.txtDialogueFileName->clear();
}

void QDialogueFileWindow::FormAccepted()
{
	if (_editDialogueFile)
	{
		_addDialogueFile->RenameDialogueFile(_editDialogueFile, ui.lblFileName->text().toStdString());
		emit FileEdited(ui.lblFileName->text().toStdString());
	}
	else if (_addDialogueFile)
	{
		auto NewDiag = _addDialogueFile->NewDialogueFile(ui.lblFileName->text().toStdString());
		emit FileAdded(NewDiag->GetFilename());
	}

	ResetForm();
	//accept();
}

void QDialogueFileWindow::Close()
{
	ResetForm();
	reject();
}

void QDialogueFileWindow::Validate()
{
	std::string EnteredDiagFileName = ui.txtDialogueFileName->text().toStdString();
	std::string FinalDiagFileName = (EnteredDiagFileName.length() > 0 ? "diag_" + EnteredDiagFileName : "diag") + ".txt";

	ui.lblFileName->setText(FinalDiagFileName.c_str());

	if (EnteredDiagFileName.find_first_not_of("abcdefghijklmnopqrstuvwxyz1234567890_") != EnteredDiagFileName.npos)
	{
		MakeError("Character name contains illegal characters.");
		return;
	}

	if (_editDialogueFile != nullptr)
	{
		auto &Diags = _addDialogueFile->DialogueFiles();
		auto Res = Diags.find(FinalDiagFileName);
		if (Res != Diags.end() && (*Res).second != _editDialogueFile)
		{
			MakeError("Dialogue file name already exists.");
			return;
		}
	}
	else if (_addDialogueFile != nullptr)
	{
		auto &Diags = _addDialogueFile->DialogueFiles();
		if (Diags.find(FinalDiagFileName) != Diags.end())
		{
			MakeError("Dialogue file name already exists.");
			return;
		}
	}

	MakeClean();
}

void QDialogueFileWindow::NewDialogueFile(Character *Char)
{
	_addDialogueFile = Char;

	this->setWindowTitle("New Dialogue File");
	this->Validate();
	this->show();
}

void QDialogueFileWindow::EditDialogueFile(Character *Char, DialogueFile *Dlg)
{
	std::string Filename = Dlg->GetFilename();
	if (Filename != "diag.txt")
	{
		size_t Start = Filename.find("diag_");
		size_t End = Filename.find(".txt");

		if (Start != 0 || Start == Filename.npos || End == Filename.npos)
			return;

		Filename = Filename.substr(5, End - 5);
	}
	else
	{
		Filename = "";
	}

	_addDialogueFile = Char;
	_editDialogueFile = Dlg;

	ui.txtDialogueFileName->setText(Filename.c_str());

	this->setWindowTitle("Edit Dialogue File");
	this->Validate();
	this->show();
}