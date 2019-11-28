#include "QReferenceEditWindow.h"

#include <QEvent>
#include <QMessageBox>
#include <QWidget>

#include <QTextBlock>

#include <tao\pegtl.hpp>

#include "Node.h"

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

		ui.btnAccept->setDisabled(false);
	}

	// ReferenceDatabase check here
	
	MakeClean();
	return true;
}

void QReferenceEditWindow::Close()
{
	if (bChangesMade && QMessageBox::warning(this, "Unsaved Changes", "Any changes will be discarded.\nContinue?", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel) == QMessageBox::StandardButton::Cancel)
		return;

	ResetForm();
	reject();
}

void QReferenceEditWindow::ResetForm()
{
	_dialogue = nullptr;
	
	ui.txtReferenceEdit->clear();	// Order is important here
	bChangesMade = false;			// or else this will be set to true

	MakeClean();
}

void QReferenceEditWindow::FormAccepted()
{
	_dialogue->SetReference(ui.txtReferenceEdit->text().toStdString());

	ResetForm();
}

QReferenceEditWindow::QReferenceEditWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	bErroneous = !IsValidReference();

	connect(ui.txtReferenceEdit, &QLineEdit::textChanged, [this]() {
		bErroneous = !IsValidReference();

		if (this->isVisible())
		{
			if (!bChangesMade)
				setWindowTitle(windowTitle().append("*"));
			bChangesMade = true;
		}
	});

	connect(this, &QDialog::accepted, this, &QReferenceEditWindow::FormAccepted);
	connect(ui.btnAccept, &QPushButton::clicked, [this]() {
		if (bErroneous)
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
}

void QReferenceEditWindow::editReference(NodeDialogue *EditNode)
{
	_dialogue = EditNode;
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
	QReferenceEditWindow RefEdit(Parent);
	RefEdit.setWindowTitle("New Reference");

	int Res = RefEdit.exec();
	if (Res == QDialog::Accepted)
		Result = RefEdit.ui.txtReferenceEdit->text().toStdString();
	return Res;
}

int QReferenceEditWindow::EditReference(QWidget *Parent, std::string &Result)
{
	QReferenceEditWindow RefEdit(Parent);
	RefEdit.setWindowTitle("Edit Reference");

	return RefEdit.exec();
}