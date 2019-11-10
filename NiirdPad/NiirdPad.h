#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include "ui_NiirdPad.h"

#include "ImportWorker.h"
#include "QScriptEditWindow.h"
#include "ScriptEngine.h"

#include "Project.h"

class NiirdPad : public QMainWindow
{
	Q_OBJECT

private:
	Ui::NiirdPadClass ui;
	//QMessageBox *_importMessageBox;
	ScriptEngine _scriptEngine;
	QScriptEditWindow *_scriptEditWindow;

	Project *_loadedProject;

	void Import();
	void ResetCharacterCombo();
	void ResetDialogueFileCombo();

public slots:
	void ImportConfirmationMessageBox(std::vector<std::string> Warnings, RawProjectFile Files);

public:
	NiirdPad(QWidget *parent = Q_NULLPTR);
	QScriptEditWindow *ScriptEditWindow();
};