#include "QCommentEditWindow.h"

#include <QMessageBox>
#include <QTextBlock>
#include <QTextDocument>

#include "Node.h"

void QCommentEditWindow::FormAccepted()
{
	if (_Node)
	{
		_Node->SetComment(ui.txtComment->toPlainText().toStdString());
	}

	ResetForm();
}

void QCommentEditWindow::Close()
{
	if (_bChangesMade && QMessageBox::warning(this, "Unsaved Changes", "Any changes will be discarded.\nContinue?", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel) == QMessageBox::StandardButton::Cancel)
		return;

	reject();
	ResetForm();
}

void QCommentEditWindow::ResetForm()
{
	ui.txtComment->clear();

	_Node = nullptr;

	_bChangesMade = false;
}

QCommentEditWindow::QCommentEditWindow(QWidget *parent) : 
	QDialog(parent)
{
	ui.setupUi(this);

	connect(this, &QDialog::accepted, this, &QCommentEditWindow::FormAccepted);
	connect(ui.btnAccept, &QPushButton::clicked, [this]()
	{
		accept();
	});

	connect(ui.btnCancel, &QPushButton::clicked, this, &QCommentEditWindow::Close);
	connect(ui.txtComment, &QTextEdit::textChanged, [this]() {
		if (this->_bIgnoreTextChanges)
			return;
		
		if (this->isVisible())
		{
			if (!_bChangesMade)
				setWindowTitle(windowTitle().append("*"));
			_bChangesMade = true;
		}

		this->_bIgnoreTextChanges = true;
		QString Text = ui.txtComment->toPlainText();
		Text.remove(QChar('\n'));

		auto Cursor = ui.txtComment->textCursor();
		ui.txtComment->setText(Text);
		ui.txtComment->setTextCursor(Cursor);
		this->_bIgnoreTextChanges = false;
	});
}

QCommentEditWindow::~QCommentEditWindow()
{

}

void QCommentEditWindow::closeEvent(QCloseEvent *event)
{
	event->ignore();
	Close();
}

void QCommentEditWindow::editComment(Node *EditNode)
{
	if (!EditNode)
		return;

	_Node = EditNode;

	this->setWindowTitle("Edit Comment");
	this->ui.txtComment->setText(_Node->GetComment().c_str());

	this->show();
}