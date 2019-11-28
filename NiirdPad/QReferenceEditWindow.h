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
	NodeDialogue *_dialogue = nullptr;

	void MakeError(const std::string &Message);
	void MakeClean();
	bool IsValidReference();

public:
	// Dialog for creating a new reference. Result will be filled if the dialog is accepted.
	static int NewReference(QWidget *Parent, std::string &Result);
	static int EditReference(QWidget *Parent, std::string &Result);

	void editReference(NodeDialogue *EditNode);

	QReferenceEditWindow(QWidget *parent);
	~QReferenceEditWindow();
};
