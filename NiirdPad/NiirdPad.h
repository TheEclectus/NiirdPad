#pragma once

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include "ui_NiirdPad.h"

#include "ImportWorker.h"
#include "QCharacterWindow.h"
#include "QCharacterSelectionWindow.h"
#include "QCommentEditWindow.h"
#include "QDialogueFileWindow.h"
#include "QReferenceEditWindow.h"
#include "QScriptEditWindow.h"
#include "ScriptEngine.h"

#include "Project.h"
#include "ResourceDiscriminator.h"

#define NIIRDPAD_VERSION	"0.9.15"

#ifndef NIIRDPAD_BUILD_ID
	#define NIIRDPAD_BUILD_ID	"99f8e58d" //"BUILD ID UNDEFINED"
#endif

class NiirdPad : public QMainWindow
{
	Q_OBJECT

private:
	Ui::NiirdPadClass ui;
	//QMessageBox *_importMessageBox;
	ScriptEngine _scriptEngine;

	QCharacterWindow *_characterWindow;
	QCharacterSelectionWindow *_characterSelectionWindow;
	QCommentEditWindow *_commentEditWindow;
	QScriptEditWindow *_scriptEditWindow;
	QReferenceEditWindow *_referenceEditWindow;
	QDialogueFileWindow *_dialogueFileWindow;

	Project *_loadedProject;

	void Import();
	bool ImportExperimental();

	void SetWindowTitle();
	void closeEvent(QCloseEvent *Event) override;

public slots:
	void ImportConfirmationMessageBox(std::vector<std::string> Warnings, RawProjectFile Files);
	void ResourceSelectionFollowup(ResourceDiscriminator::Results Results, std::vector<std::string> Warnings);

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