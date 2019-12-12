#include "QReferenceEditWindow.h"

#include <QEvent>
#include <QMessageBox>
#include <QTextBlock>
#include <QWidget>

#include <tao\pegtl.hpp>

#include "Node.h"
#include "ReferenceDatabase.h"

void QReferenceEditWindow::MakeError(const std::string &Message)
{
	ui.lblInfo->setText(Message.c_str());

	QPalette Pal = ui.lblInfo->palette();
	Pal.setColor(ui.lblInfo->foregroundRole(), Qt::red);
	ui.lblInfo->setPalette(Pal);
}

void QReferenceEditWindow::MakeClean()
{
	ui.lblInfo->setText("No errors found.");
	QPalette Pal = ui.lblInfo->palette();
	Pal.setColor(ui.lblInfo->foregroundRole(), Qt::darkGreen);
	ui.lblInfo->setPalette(Pal);
}

bool QReferenceEditWindow::IsValidReference()
{
	if (ui.txtReferenceEdit->text().length() == 0)
	{
		MakeError("Reference cannot be of zero length.");
		ui.btnAccept->setDisabled(true);
		return false;
	}
	else
	{
		std::string Text = ui.txtReferenceEdit->text().toStdString() + "\n";
		tao::pegtl::string_input<> Input(Text, "");
		try
		{
			tao::pegtl::parse< tao::pegtl::must< tao::pegtl::plus< tao::pegtl::sor< tao::pegtl::alnum, tao::pegtl::one<'_'> > >, tao::pegtl::one<'\n'> > >(Input, Text);
		}
		catch (std::exception Exception)
		{
			MakeError("Reference contains illegal characters.");
			ui.btnAccept->setDisabled(true);
			return false;
		}

		auto Res = _database->Find(ui.txtReferenceEdit->text().toStdString());
		if (Res != nullptr && Res != _dialogue)
		{
			MakeError("Reference already exists.");
			ui.btnAccept->setDisabled(true);
			return false;
		}

		ui.btnAccept->setDisabled(false);
	}

	// ReferenceDatabase check here
	
	MakeClean();
	return true;
}

void QReferenceEditWindow::Close()
{
	if (_bChangesMade && QMessageBox::warning(this, "Unsaved Changes", "Any changes will be discarded.\nContinue?", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel) == QMessageBox::StandardButton::Cancel)
		return;

	ResetForm();
	reject();
}

void QReferenceEditWindow::ResetForm()
{
	_dialogue = nullptr;
	_destNode = nullptr;
	
	ui.txtReferenceEdit->clear();	// Order is important here
	_bChangesMade = false;			// or else this will be set to true

	MakeClean();
}

void QReferenceEditWindow::FormAccepted()
{
	if (_dialogue != nullptr)
	{
		if (_database->Rename(_dialogue->GetReference(), ui.txtReferenceEdit->text().toStdString()))
		{
			_dialogue->SetReference(ui.txtReferenceEdit->text().toStdString());

			ResetForm();
		}
		else
		{
			QMessageBox::warning(this, "Warning", "Something went wrong and QReferenceDatabase::Rename() didn't work.\nTell Eclip.", QMessageBox::StandardButton::Ok);
			
			ResetForm();
		}
	}
	else if (_destNode)
	{
		std::string NewReference = ui.txtReferenceEdit->text().toStdString();
		if (_database->Find(NewReference) == nullptr)
		{
			emit NewReferenceCreated(NewReference, _destNode);

			ResetForm();
		}
		else
		{
			QMessageBox::warning(this, "Warning", "Something went wrong and an existing reference name was allowed to pass.\nTell Eclip.", QMessageBox::StandardButton::Ok);

			ResetForm();
		}
	}
}

QReferenceEditWindow::QReferenceEditWindow(QWidget *Parent) :
	QDialog(Parent)
{
	ui.setupUi(this);

	_bErroneous = !IsValidReference();

	connect(ui.txtReferenceEdit, &QLineEdit::textChanged, [this]() {
		_bErroneous = !IsValidReference();

		if (this->isVisible())
		{
			if (!_bChangesMade)
				setWindowTitle(windowTitle().append("*"));
			_bChangesMade = true;
		}
	});

	connect(this, &QDialog::accepted, this, &QReferenceEditWindow::FormAccepted);
	connect(ui.btnAccept, &QPushButton::clicked, [this]() {
		if (_bErroneous)
		{
			if (QMessageBox::warning(this, "Warning", "There are errors with this reference name.\nContinue anyways?", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel) == QMessageBox::StandardButton::Cancel)
			{
				return;
			}
		}

		accept();
	});
	connect(ui.btnCancel, &QPushButton::clicked, this, &QReferenceEditWindow::Close);
}

QReferenceEditWindow::~QReferenceEditWindow()
{

}

void QReferenceEditWindow::closeEvent(QCloseEvent *Event)
{
	Event->ignore();
	Close();
}

void QReferenceEditWindow::newReference(Node *Dest, ReferenceDatabase &Database)
{
	_destNode = Dest;
	_database = &Database;

	this->setWindowTitle(QString("New Reference"));
	ui.txtReferenceEdit->setText("");

	IsValidReference();
	this->show();
}

void QReferenceEditWindow::editReference(NodeDialogue *EditNode, ReferenceDatabase &Database)
{
	_dialogue = EditNode;
	_database = &Database;

	this->setWindowTitle(QString("Edit Reference [").append(EditNode->GetReference().c_str()).append("]"));
	ui.txtReferenceEdit->setText(EditNode->GetReference().c_str());

	this->show();

	/*int Res = exec();
	if (Res == QDialog::Accepted)
		Result = this->ui.txtReferenceEdit->text().toStdString();
	return Res;*/
}

int QReferenceEditWindow::NewReference(QWidget *Parent, std::string &Result)
{
	/*ReferenceDatabase Dummy;
	QReferenceEditWindow RefEdit(Parent);
	RefEdit.setWindowTitle("New Reference");

	int Res = RefEdit.exec();
	if (Res == QDialog::Accepted)
		Result = RefEdit.ui.txtReferenceEdit->text().toStdString();
	return Res;*/

	return 0;
}

int QReferenceEditWindow::EditReference(QWidget *Parent, std::string &Result)
{
	/*ReferenceDatabase Dummy;
	QReferenceEditWindow RefEdit(Parent);
	RefEdit.setWindowTitle("Edit Reference");

	return RefEdit.exec();*/

	return 0;
}