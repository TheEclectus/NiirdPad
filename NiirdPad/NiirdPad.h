#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include "ui_NiirdPad.h"

#include "ImportWorker.h"
#include "QCharacterWindow.h"
#include "QCommentEditWindow.h"
#include "QReferenceEditWindow.h"
#include "QScriptEditWindow.h"
#include "ScriptEngine.h"

#include "Project.h"

#define NIIRDPAD_VERSION	"0.9.14"

#ifndef NIIRDPAD_BUILD_ID
	#define NIIRDPAD_BUILD_ID	"f4d20de4" //"BUILD ID UNDEFINED"
#endif

class NiirdPad : public QMainWindow
{
	Q_OBJECT

private:
	Ui::NiirdPadClass ui;
	//QMessageBox *_importMessageBox;
	ScriptEngine _scriptEngine;
	QCharacterWindow *_characterWindow;
	QCommentEditWindow *_commentEditWindow;
	QScriptEditWindow *_scriptEditWindow;
	QReferenceEditWindow *_referenceEditWindow;

	Project *_loadedProject;

	void Import();
	void SetWindowTitle();
	void closeEvent(QCloseEvent *Event) override;

public slots:
	void ImportConfirmationMessageBox(std::vector<std::string> Warnings, RawProjectFile Files);

public:
	NiirdPad(QWidget *parent = Q_NULLPTR);
	QCommentEditWindow *CommentEditWindow();
	QScriptEditWindow *ScriptEditWindow();
	QReferenceEditWindow *ReferenceEditWindow();
	ScriptEngine &ScriptEngine();

	void ResetCharacterCombo();
	void ResetDialogueFileCombo();
	void ResetIndexCombo();

	void DirtyProjectChanges();
	void CleanProjectChanges();
	Project *LoadedProject();
};