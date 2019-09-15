#include "QScriptEditWindow.h"

#include <QTextBlock>
#include <QTextDocument>

#include <strtk.hpp>

#include "Node.h"

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

int QScriptEditWindow::EditDialogueFragment(QWidget *Parent, ScriptEngine &Engine, NodeDialogue &Dialogue)
{
	QScriptEditWindow ScriptEdit(Parent, Engine, true);
	
	int Res = ScriptEdit.exec();
	if (Res == QDialog::Accepted)
	{
		QTextDocument &QtDoc = *ScriptEdit.ui.txtScripts->document();
		std::vector<std::string> FunctionLines = {};
		for (QTextBlock CurLine = QtDoc.begin(); CurLine != QtDoc.end(); CurLine = CurLine.next())
		{
			FunctionLines.push_back(CurLine.text().toStdString());
		}
		strtk::remove_empty_strings(FunctionLines);

		std::string DialogueText = ScriptEdit.ui.txtText->toPlainText().toStdString();

		Dialogue.SetAll(FunctionLines, DialogueText);

		// TODO: remove trailing newlines?
		size_t Pos = DialogueText.find('\n');
		while (Pos != std::string::npos)
		{
			DialogueText.replace(Pos, 1, "<br>");
			Pos = DialogueText.find('\n');
		}
	}

	return Res;
}

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