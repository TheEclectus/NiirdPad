#include "QReferenceEditWindow.h"

#include <QMessageBox>

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
		ui.btnAccept->setDisabled(false);
	}

	// ReferenceDatabase check here
	
	MakeClean();
	return true;
}

QReferenceEditWindow::QReferenceEditWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	bErroneous = !IsValidReference();

	connect(ui.txtReferenceEdit, &QLineEdit::textChanged, [this]() {
		bErroneous = !IsValidReference();
	});

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
	connect(ui.btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

QReferenceEditWindow::~QReferenceEditWindow()
{
}

int QReferenceEditWindow::NewReference(QWidget *Parent, std::string &Result)
{
	QReferenceEditWindow RefEdit(Parent);
	RefEdit.setWindowTitle("New Reference");
	RefEdit.IsValidReference();

	return RefEdit.exec();
}