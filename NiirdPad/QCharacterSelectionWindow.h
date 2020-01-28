#pragma once

#include <QDialog>
#include "ui_QCharacterSelectionWindow.h"

class QCharacterSelectionWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::QCharacterSelectionWindow ui;

	/*
		CUSTOM TRI-STATE IMPLEMENTATION

		Whenever an itme is clicked:
		1. If it doesn't match the desired state, set it.
		2. If it's a top-level item...
			1. Modify all child widgets to match its state without affecting its parent.
		3. If it's a child item...
			1. Alter the parent item without affecting the children.
				If all children are checked, check.
				If some children are checked, half-check.
				If no children are checked, uncheck.
	*/

	// Just sets the check state.
	void SetCheckState(QTreeWidgetItem *Item, Qt::CheckState State);
	void ProcessCheck(QTreeWidgetItem *Item, Qt::CheckState State);

	//QTreeWidgetItem *FindTLInTree(QTreeWidget *Widget, QString &Label);
	//QTreeWidgetItem *FindItemInTL(QTreeWidgetItem *TL, QString &Label);

	//void UncheckTreeWidgetItem(QTreeWidgetItem *Item) const;

public:
	QCharacterSelectionWindow(QWidget *parent = Q_NULLPTR);
	~QCharacterSelectionWindow();

	void UpdateChecks();
};
