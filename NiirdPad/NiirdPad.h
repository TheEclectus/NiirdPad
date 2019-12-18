#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include "ui_NiirdPad.h"

#include "ImportWorker.h"
#include "QCommentEditWindow.h"
#include "QReferenceEditWindow.h"
#include "QScriptEditWindow.h"
#include "ScriptEngine.h"

#include "Project.h"

#define NIIRDPAD_VERSION	"0.9.1"

#ifndef NIIRDPAD_BUILD_ID
	#define NIIRDPAD_BUILD_ID	"999a2fec" //"BUILD ID UNDEFINED"
#endif

class NiirdPad : public QMainWindow
{
	Q_OBJECT

private:
	Ui::NiirdPadClass ui;
	//QMessageBox *_importMessageBox;
	ScriptEngine _scriptEngine;
	QCommentEditWindow *_commentEditWindow;
	QScriptEditWindow *_scriptEditWindow;
	QReferenceEditWindow *_referenceEditWindow;

	Project *_loadedProject;

	void Import();
	void ResetCharacterCombo();
	void ResetDialogueFileCombo();
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

	void DirtyProjectChanges();
	void CleanProjectChanges();
};