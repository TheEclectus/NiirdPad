#pragma once

#include <QDialog>
#include "ui_QCharacterWindow.h"

// Character.h
class Character;

// Project.h
class Project;

class QCharacterWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::QCharacterWindow ui;
	Character *_editCharacter = nullptr;
	Project *_addCharacter = nullptr;

	void MakeError(const std::string &Message);
	void MakeClean();

public:
	QCharacterWindow(QWidget *parent = Q_NULLPTR);
	~QCharacterWindow();
	void closeEvent(QCloseEvent *event) override;

	void ResetForm();
	void FormAccepted();
	void Close();

	void Validate();

	void NewCharacter(Project *Proj);
	void EditCharacter(Project *Proj, Character *Char);

signals:
	void NameEdited(std::string);
	void NameAdded(std::string);
};
