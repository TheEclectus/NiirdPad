#include "QScriptEditWindow.h"

#include <QTextBlock>
#include <QTextDocument>

#include <strtk.hpp>

#include "Node.h"

#include <thread>

// Consider further retooling, pertaining to the proper fade-in of the window when it's reopened.

QScriptEditWindow::QScriptEditWindow(QWidget *parent, ScriptEngine &Engine, bool bHideVisConditions) :
	QDialog(parent),
	_scriptEngine(Engine)
{
	ui.setupUi(this);

	//this->setWindowFlags(Qt::Dialog | Qt::Desktop);

	//ui.splitter->setCollapsible(0, true);
	//ui.splitter->child
	if (bHideVisConditions)
	{
		ui.txtVisibility->setEnabled(false);
		ui.txtVisibility->hide();
		ui.lblVisErrors->setEnabled(false);
		ui.lblVisErrors->hide();

		ui.splitter->handle(1)->setEnabled(false);
		ui.splitter->handle(1)->setVisible(false);
	}

	connect(this, &QDialog::accepted, this, &QScriptEditWindow::FormAccepted);
	connect(ui.btnAccept, &QPushButton::clicked, [this]()
	{
		accept();
	});
	connect(ui.btnCancel, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui.txtScripts, &QTextEdit::textChanged, this, &QScriptEditWindow::ResetTimer);
	connect(ui.txtVisibility, &QTextEdit::textChanged, this, &QScriptEditWindow::ResetTimer);

	connect(&_updateTimer, &QTimer::timeout, this, &QScriptEditWindow::ValidateScripts);
	connect(&_updateTimer, &QTimer::timeout, this, &QScriptEditWindow::ValidateVisConditions);

	ValidateScripts();
	ValidateVisConditions();
}

QScriptEditWindow::~QScriptEditWindow()
{

}

//int QScriptEditWindow::EditDialogueFragment(QWidget *Parent, ScriptEngine &Engine, NodeDialogue &Dialogue)
//{
//	QScriptEditWindow ScriptEdit(Parent, Engine, true);
//
//	/*
//		1. Dialogue text needs to have "<br>"s replaced with newlines.
//		2. txtText populated with 
//		3. txtScripts populated with FunctionLines
//		4. txtVisibility populated with VisibilityLines
//	*/
//	std::string FormattedDialogue = Dialogue.GetDialogue();
//	size_t Pos = FormattedDialogue.find("<br>");
//	while (Pos != std::string::npos)
//	{
//		FormattedDialogue.replace(Pos, 4, "\n");
//		Pos = FormattedDialogue.find("<br>");
//	}
//	ScriptEdit.ui.txtText->setPlainText(FormattedDialogue.c_str());
//
//	ScriptEdit.ui.txtVisibility->clear();
//	for (auto &CurVisLine : Dialogue.GetFunctionLines())
//	{
//		ScriptEdit.ui.txtVisibility->append(CurVisLine.c_str());
//	}
//	
//	ScriptEdit.ui.txtScripts->clear();
//	for (auto &CurFuncLine : Dialogue.GetFunctionLines())
//	{
//		ScriptEdit.ui.txtScripts->append(CurFuncLine.c_str());
//	}
//	//ScriptEdit.ui.txtScripts->document()->setPlainText()
//	
//	int Res = ScriptEdit.exec();
//	if (Res == QDialog::DialogCode::Accepted)
//	{
//		QTextDocument &QtDoc = *ScriptEdit.ui.txtScripts->document();
//		std::vector<std::string> FunctionLines = {};
//		for (QTextBlock CurLine = QtDoc.begin(); CurLine != QtDoc.end(); CurLine = CurLine.next())
//		{
//			FunctionLines.push_back(CurLine.text().toStdString());
//		}
//		strtk::remove_empty_strings(FunctionLines);
//
//		std::string DialogueText = ScriptEdit.ui.txtText->toPlainText().toStdString();
//
//		// TODO: remove trailing newlines?
//		size_t Pos = DialogueText.find('\n');
//		while (Pos != std::string::npos)
//		{
//			DialogueText.replace(Pos, 1, "<br>");
//			Pos = DialogueText.find('\n');
//		}
//		Dialogue.SetAll(FunctionLines, DialogueText);
//	}
//
//	return Res;
//}

int QScriptEditWindow::EditOptionFragment(QWidget *Parent, ScriptEngine &Engine, NodeOption &Option)
{
	QScriptEditWindow ScriptEdit(Parent, Engine, false);

	int Res = ScriptEdit.exec();
	if (Res == QDialog::Accepted)
	{
		QTextDocument &QtDoc = *ScriptEdit.ui.txtVisibility->document();
		std::vector<std::string> VisLines = {};
		for (QTextBlock CurLine = QtDoc.begin(); CurLine != QtDoc.end(); CurLine = CurLine.next())
		{
			VisLines.push_back(CurLine.text().toStdString());
		}
		strtk::remove_empty_strings(VisLines);

		QTextDocument &QtDocScripts = *ScriptEdit.ui.txtScripts->document();
		std::vector<std::string> ScriptLines = {};
		for (QTextBlock CurLine = QtDocScripts.begin(); CurLine != QtDocScripts.end(); CurLine = CurLine.next())
		{
			ScriptLines.push_back(CurLine.text().toStdString());
		}
		strtk::remove_empty_strings(ScriptLines);

		std::string OptionText = ScriptEdit.ui.txtText->toPlainText().toStdString();
		// TODO: remove trailing newlines?
		size_t Pos = OptionText.find('\n');
		while (Pos != std::string::npos)
		{
			OptionText.replace(Pos, 1, "<br>");
			Pos = OptionText.find('\n');
		}

		Option.SetAll(VisLines, ScriptLines, OptionText);
	}

	return Res;
}

void QScriptEditWindow::closeEvent(QCloseEvent *event)
{
	// Same logic as btnCancel goes here
	event->ignore();
}

void QScriptEditWindow::dialogueFragment(NodeDialogue *Dialogue)
{
	if (!Dialogue)
		return;

	_dialogue = Dialogue;

	ui.txtVisibility->setEnabled(false);
	ui.txtVisibility->hide();
	ui.lblVisErrors->setEnabled(false);
	ui.lblVisErrors->hide();

	ui.splitter->handle(1)->setEnabled(false);
	ui.splitter->handle(1)->setVisible(false);

	/*
		1. Dialogue text needs to have "<br>"s replaced with newlines.
		2. txtText populated with
		3. txtScripts populated with FunctionLines
		4. txtVisibility populated with VisibilityLines
	*/
	std::string FormattedDialogue = _dialogue->GetDialogue();
	size_t Pos = FormattedDialogue.find("<br>");
	while (Pos != std::string::npos)
	{
		FormattedDialogue.replace(Pos, 4, "\n");
		Pos = FormattedDialogue.find("<br>");
	}
	this->ui.txtText->setPlainText(FormattedDialogue.c_str());

	/*this->ui.txtVisibility->clear();
	for (auto &CurVisLine : _dialogue->GetFunctionLines())
	{
		this->ui.txtVisibility->append(CurVisLine.c_str());
	}*/

	this->ui.txtScripts->clear();
	for (auto &CurFuncLine : _dialogue->GetFunctionLines())
	{
		this->ui.txtScripts->append(CurFuncLine.c_str());
	}

	//this->activateWindow();
	this->show();
}

void QScriptEditWindow::optionFragment(NodeOption *Option)
{
	if (!Option)
		return;

	_option = Option;

	ui.txtVisibility->setEnabled(true);
	ui.txtVisibility->show();
	ui.lblVisErrors->setEnabled(true);
	ui.lblVisErrors->hide();

	ui.splitter->handle(1)->setEnabled(true);
	ui.splitter->handle(1)->setVisible(true);

	/*
		1. Dialogue text needs to have "<br>"s replaced with newlines.
		2. txtText populated with
		3. txtScripts populated with FunctionLines
		4. txtVisibility populated with VisibilityLines
	*/
	std::string FormattedOption = _option->GetOption();
	size_t Pos = FormattedOption.find("<br>");
	while (Pos != std::string::npos)
	{
		FormattedOption.replace(Pos, 4, "\n");
		Pos = FormattedOption.find("<br>");
	}
	this->ui.txtText->setPlainText(FormattedOption.c_str());

	this->ui.txtVisibility->clear();
	for (auto &CurVisLine : _option->GetFunctionLines())
	{
		this->ui.txtVisibility->append(CurVisLine.c_str());
	}

	this->ui.txtScripts->clear();
	for (auto &CurFuncLine : _option->GetFunctionLines())
	{
		this->ui.txtScripts->append(CurFuncLine.c_str());
	}

	this->show();
}

void QScriptEditWindow::ResetTimer()
{
	_updateTimer.start(UPDATE_DELAY_MS);
}

void QScriptEditWindow::ValidateScripts()
{
	//ui.txtErrors->clear();

	QTextDocument &QtDoc = *ui.txtScripts->document();
	std::vector<std::string> Lines = {};
	for (QTextBlock CurLine = QtDoc.begin(); CurLine != QtDoc.end(); CurLine = CurLine.next())
	{
		Lines.push_back(CurLine.text().toStdString());
	}

	bool bEncounteredError = false;
	bool bHasIndexModifyingFunction = false;
	for (auto Line = Lines.begin(); Line != Lines.end(); Line++)
	{
		//std::string LineMinusWhitespace
		if (Line->length() == 0) continue;

		std::vector<std::string> Keys = {};
		std::string Err = "";
		if (!_scriptEngine.bScriptIsValid(*Line, Keys, Err))
		{
			//ui.lblScriptErrors->setText(Err.c_str());
			ScriptsMakeError(Err);
			bEncounteredError = true;
			break;
		}

		if (Keys.size() > 0)
		{
			if (!bHasIndexModifyingFunction)
				bHasIndexModifyingFunction = true;
			else
			{
				ScriptsMakeError("Multiple index-modifying functions defined.");
				bEncounteredError = true;
				break;
			}
		}
	}

	if(!bEncounteredError)
		ScriptsMakeClean();

	//ui.txtErrors->insertPlainText(">blap\n");

	_updateTimer.stop();
}

void QScriptEditWindow::ScriptsMakeClean()
{
	ui.lblScriptErrors->setText("No errors found.");

	QPalette Pal = ui.lblScriptErrors->palette();
	Pal.setColor(ui.lblScriptErrors->foregroundRole(), Qt::darkGreen);
	ui.lblScriptErrors->setPalette(Pal);
}

void QScriptEditWindow::ScriptsMakeError(const std::string &Message)
{
	ui.lblScriptErrors->setText(Message.c_str());

	QPalette Pal = ui.lblScriptErrors->palette();
	Pal.setColor(ui.lblScriptErrors->foregroundRole(), Qt::red);
	ui.lblScriptErrors->setPalette(Pal);
}

void QScriptEditWindow::ValidateVisConditions()
{
	QTextDocument &QtDoc = *ui.txtVisibility->document();
	std::vector<std::string> Lines = {};
	for (QTextBlock CurLine = QtDoc.begin(); CurLine != QtDoc.end(); CurLine = CurLine.next())
	{
		Lines.push_back(CurLine.text().toStdString());
	}

	bool bEncounteredError = false;
	bool bHasIndexModifyingFunction = false;
	for (auto Line = Lines.begin(); Line != Lines.end(); Line++)
	{
		//std::string LineMinusWhitespace
		if (Line->length() == 0) continue;

		std::string Err = "";
		if (!_scriptEngine.bVisConditionIsValid(*Line, Err))
		{
			//ui.lblScriptErrors->setText(Err.c_str());
			VisMakeError(Err);
			bEncounteredError = true;
			break;
		}
	}

	if (!bEncounteredError)
		VisMakeClean();

	//ui.txtErrors->insertPlainText(">blap\n");

	_updateTimer.stop();
}

void QScriptEditWindow::VisMakeClean()
{
	ui.lblVisErrors->setText("No errors found.");

	QPalette Pal = ui.lblVisErrors->palette();
	Pal.setColor(ui.lblVisErrors->foregroundRole(), Qt::darkGreen);
	ui.lblVisErrors->setPalette(Pal);
}

void QScriptEditWindow::VisMakeError(const std::string &Message)
{
	ui.lblVisErrors->setText(Message.c_str());

	QPalette Pal = ui.lblVisErrors->palette();
	Pal.setColor(ui.lblVisErrors->foregroundRole(), Qt::red);
	ui.lblVisErrors->setPalette(Pal);
}

void QScriptEditWindow::ResetForm()
{
	ui.txtScripts->clear();
	ui.txtText->clear();
	ui.txtVisibility->clear();

	VisMakeClean();
	ScriptsMakeClean();
}

void QScriptEditWindow::FormAccepted()
{
	if (!_dialogue && !_option)
		return;

	if (_dialogue)
	{
		QTextDocument &QtDoc = *this->ui.txtScripts->document();
		std::vector<std::string> FunctionLines = {};
		for (QTextBlock CurLine = QtDoc.begin(); CurLine != QtDoc.end(); CurLine = CurLine.next())
		{
			FunctionLines.push_back(CurLine.text().toStdString());
		}
		strtk::remove_empty_strings(FunctionLines);

		std::string DialogueText = this->ui.txtText->toPlainText().toStdString();

		// TODO: remove trailing newlines?
		size_t Pos = DialogueText.find('\n');
		while (Pos != std::string::npos)
		{
			DialogueText.replace(Pos, 1, "<br>");
			Pos = DialogueText.find('\n');
		}
		_dialogue->SetAll(FunctionLines, DialogueText);

		_dialogue = nullptr;
	}
	else if (_option)
	{
		QTextDocument &QtDoc = *this->ui.txtScripts->document();
		std::vector<std::string> FunctionLines = {};
		for (QTextBlock CurLine = QtDoc.begin(); CurLine != QtDoc.end(); CurLine = CurLine.next())
		{
			FunctionLines.push_back(CurLine.text().toStdString());
		}
		strtk::remove_empty_strings(FunctionLines);

		QTextDocument &VisDoc = *this->ui.txtVisibility->document();
		std::vector<std::string> VisLines = {};
		for (QTextBlock CurLine = VisDoc.begin(); CurLine != VisDoc.end(); CurLine = CurLine.next())
		{
			VisLines.push_back(CurLine.text().toStdString());
		}
		strtk::remove_empty_strings(VisLines);

		std::string OptionText = this->ui.txtText->toPlainText().toStdString();

		// TODO: remove trailing newlines?
		size_t Pos = OptionText.find('\n');
		while (Pos != std::string::npos)
		{
			OptionText.replace(Pos, 1, "<br>");
			Pos = OptionText.find('\n');
		}
		_option->SetAll(VisLines, FunctionLines, OptionText);

		_option = nullptr;
	}

	ResetForm();
	this->hide();
}