#pragma once

#include <QDialog>
#include <qtimer.h>
#include "ui_QScriptEditWindow.h"

#include "ScriptEngine.h"

// Node.h
class NodeDialogue;
class NodeOption;

class QScriptEditWindow : public QDialog
{
	Q_OBJECT

private:
	const int UPDATE_DELAY_MS = 500;

	Ui::QScriptEditWindow ui;
	QTimer _updateTimer;
	const ScriptEngine &_scriptEngine;

	NodeDialogue *_dialogue;
	NodeOption *_option;
	//std::vector<std::string> _keys;

	bool _bChangesMade = false;

	void ResetTimer();
	
	void ValidateScripts();
	void ScriptsMakeClean();
	void ScriptsMakeError(const std::string &Message);

	void ValidateVisConditions();
	void VisMakeClean();
	void VisMakeError(const std::string &Message);

	void Close();
	void ResetForm();
	void FormAccepted();

public:
	//static int EditDialogueFragment(QWidget *Parent, ScriptEngine &Engine, NodeDialogue &Dialogue);
	static int EditOptionFragment(QWidget *Parent, ScriptEngine &Engine, NodeOption &Option);

	QScriptEditWindow(QWidget *parent, ScriptEngine &Engine, bool bHideVisConditions);
	~QScriptEditWindow();
	void closeEvent(QCloseEvent *event) override;

	/// <summary>
	/// Shows the window to edit a NodeDialogue.
	/// </summary>
	/// <param name="Dialogue"></param>
	void dialogueFragment(NodeDialogue *Dialogue);

	/// <summary>
	/// Shows the window to edit a NodeOption.
	/// </summary>
	/// <param name="Option"></param>
	void optionFragment(NodeOption *Option);
};
