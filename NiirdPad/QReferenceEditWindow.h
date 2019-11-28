#pragma once

#include <QDialog>
#include "ui_QReferenceEditWindow.h"

// Node.h
class NodeDialogue;

class QReferenceEditWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::QReferenceEditWindow ui;
	bool bErroneous = false;
	bool bChangesMade = false;
	NodeDialogue *_dialogue = nullptr;

	void MakeError(const std::string &Message);
	void MakeClean();
	bool IsValidReference();

	void Close();
	void ResetForm();
	void FormAccepted();

public:
	// Dialog for creating a new reference. Result will be filled if the dialog is accepted.
	static int NewReference(QWidget *Parent, std::string &Result);
	static int EditReference(QWidget *Parent, std::string &Result);

	QReferenceEditWindow(QWidget *parent);
	~QReferenceEditWindow();
	void closeEvent(QCloseEvent *event) override;

	void editReference(NodeDialogue *EditNode);
};
