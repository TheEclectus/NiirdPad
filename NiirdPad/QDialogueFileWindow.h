#pragma once

#include <QDialog>
#include "ui_QDialogueFileWindow.h"

// DialogueFileh
class DialogueFile;

// Character.h
class Character;

class QDialogueFileWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::QDialogueFileWindow ui;
	DialogueFile *_editDialogueFile = nullptr;
	Character *_addDialogueFile = nullptr;

	void MakeError(const std::string &Message);
	void MakeClean();

public:
	QDialogueFileWindow(QWidget *parent = Q_NULLPTR);
	~QDialogueFileWindow();
	void closeEvent(QCloseEvent *event) override;

	void ResetForm();
	void FormAccepted();
	void Close();

	void Validate();

	void NewDialogueFile(Character *Char);
	void EditDialogueFile(Character *Char, DialogueFile *Dlg);

signals:
	void FileEdited(std::string);
	void FileAdded(std::string);
};
