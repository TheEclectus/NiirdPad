#pragma once

#include <QDialog>
#include "ui_QReferenceEditWindow.h"

class QReferenceEditWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::QReferenceEditWindow ui;
	bool bErroneous = false;

	void MakeError(const std::string &Message);
	void MakeClean();
	bool IsValidReference();

public:
	QReferenceEditWindow(QWidget *parent = Q_NULLPTR);
	~QReferenceEditWindow();

	// Dialog for creating a new reference. Result will be filled if the dialog is accepted.
	static int NewReference(QWidget *Parent, std::string &Result);
};
