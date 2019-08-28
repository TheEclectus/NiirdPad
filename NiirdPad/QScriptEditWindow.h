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

	void ResetTimer();
	
	void ValidateScripts();
	void ScriptsMakeClean();
	void ScriptsMakeError(const std::string &Message);

	void ValidateVisConditions();
	void VisMakeClean();
	void VisMakeError(const std::string &Message);

public:
	static int EditDialogueFragment(QWidget *Parent, ScriptEngine &Engine, NodeDialogue &Dialogue);
	static int EditOptionFragment(QWidget *Parent, ScriptEngine &Engine, NodeOption &Option);

	QScriptEditWindow(QWidget *parent, ScriptEngine &Engine, bool bHideVisConditions);
	~QScriptEditWindow();
	void closeEvent(QCloseEvent *event) override;
};
