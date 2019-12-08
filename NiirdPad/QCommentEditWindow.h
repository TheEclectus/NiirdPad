#pragma once

#include <QDialog>
#include "ui_QCommentEditWindow.h"

// Node.h
class Node;

class QCommentEditWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::QCommentEditWindow ui;

	Node *_Node = nullptr;
	bool _bChangesMade = false;
	bool _bIgnoreTextChanges = false;

	void FormAccepted();
	void Close();
	void ResetForm();

public:
	QCommentEditWindow(QWidget *parent);
	~QCommentEditWindow();
	void closeEvent(QCloseEvent *event) override;

	void editComment(Node *EditNode);
};
