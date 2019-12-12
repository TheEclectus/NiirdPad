#pragma once

#include <QDialog>
#include "ui_QReferenceEditWindow.h"

// Node.h
class Node;
class NodeDialogue;

// ReferenceDatabase.h
class ReferenceDatabase;

class QReferenceEditWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::QReferenceEditWindow ui;
	ReferenceDatabase *_database = nullptr;

	bool _bErroneous = false;
	bool _bChangesMade = false;
	//bool _bNewReference = false;

	NodeDialogue *_dialogue = nullptr;	// If nullptr, we're making a new reference.
	Node *_destNode = nullptr;			// Must be set if _dialogue is null.

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

	QReferenceEditWindow(QWidget *Parent);
	~QReferenceEditWindow();
	void closeEvent(QCloseEvent *event) override;

	void newReference(Node *Dest, ReferenceDatabase &Database);
	void editReference(NodeDialogue *EditNode, ReferenceDatabase &Database);

signals:
	void NewReferenceCreated(std::string, Node*);
};
