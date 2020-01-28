#include "QCharacterSelectionWindow.h"

void QCharacterSelectionWindow::SetCheckState(QTreeWidgetItem *Item, Qt::CheckState State)
{
	Item->treeWidget()->blockSignals(true);

	if(Item->checkState(0) != State)
		Item->setCheckState(0, State);

	Item->treeWidget()->blockSignals(false);
}

QTreeWidgetItem *FindTLInTree(QTreeWidget *Widget, QString &Label)
{
	for (int i = 0; i < Widget->topLevelItemCount(); i++)
	{
		if (Widget->topLevelItem(i)->text(0) == Label)
			return Widget->topLevelItem(i);
	}

	return nullptr;
}

QTreeWidgetItem *FindItemInTL(QTreeWidgetItem *TL, QString &Label)
{
	for (int i = 0; i < TL->childCount(); i++)
	{
		if (TL->child(i)->text(0) == Label)
			return TL->child(i);
	}

	return nullptr;
}

void ParentLogic(QTreeWidget *Existing, QTreeWidgetItem *Item, Qt::CheckState State)
{
	auto *PairedSideTL = FindTLInTree(Existing, Item->text(0));
	if (PairedSideTL != nullptr)
	{
		QFont ItemFont = PairedSideTL->font(0);
		switch (State)
		{
		case Qt::CheckState::Checked:
			ItemFont.setItalic(true);
			ItemFont.setStrikeOut(true);
			break;
		case Qt::CheckState::PartiallyChecked:
			ItemFont.setItalic(true);
			ItemFont.setStrikeOut(false);
			break;
		case Qt::CheckState::Unchecked:
			ItemFont.setItalic(false);
			ItemFont.setStrikeOut(false);
			break;
		}
		PairedSideTL->setFont(0, ItemFont);
	}
}

void ChildLogic(QTreeWidget *Existing, QTreeWidgetItem *Item, Qt::CheckState State)
{
	QTreeWidgetItem *ParentItem = Item->parent();

	auto *PairedSideTL = FindTLInTree(Existing, ParentItem->text(0));
	if (PairedSideTL != nullptr)
	{
		// Get the matching child item on the other side
		auto *PairedSideChild = FindItemInTL(PairedSideTL, Item->text(0));
		if (PairedSideChild != nullptr)
		{
			QFont ItemFont = PairedSideChild->font(0);
			switch (State)
			{
			case Qt::CheckState::Checked:
				ItemFont.setItalic(true);
				ItemFont.setStrikeOut(true);
				break;
			case Qt::CheckState::Unchecked:
				ItemFont.setItalic(false);
				ItemFont.setStrikeOut(false);
				break;
			}
			PairedSideChild->setFont(0, ItemFont);
		}
	}
}

void QCharacterSelectionWindow::ProcessCheck(QTreeWidgetItem *Item, Qt::CheckState State)
{
	// Set the check state
	SetCheckState(Item, State);

	// Top-level items
	if (Item->parent() == nullptr)
	{
		// Parent logic here
		ParentLogic(ui.treeExisting, Item, State);

		// Iterate through children and set their states accordingly.
		for (int i = 0; i < Item->childCount(); i++)
		{
			auto *ChildItem = Item->child(i);
			SetCheckState(ChildItem, State);

			// Apply logic here
			ChildLogic(ui.treeExisting, ChildItem, State);
		}
	}
	else // Child items
	{
		QTreeWidgetItem *ParentItem = Item->parent();
		
		SetCheckState(Item, State);
		// Apply logic here
		ChildLogic(ui.treeExisting, Item, State);

		int CheckedItems = 0;
		// Iterate through children and set their states accordingly.
		for (int i = 0; i < ParentItem->childCount(); i++)
		{
			//SetCheckState(ParentItem->child(i), State);
			if(ParentItem->child(i)->checkState(0) == Qt::CheckState::Checked)
				CheckedItems++;
		}

		Qt::CheckState ParentCheckState = Qt::CheckState::Unchecked;
		if (CheckedItems > 0 && CheckedItems < ParentItem->childCount())
		{
			ParentCheckState = Qt::CheckState::PartiallyChecked;
		}
		else if(CheckedItems == ParentItem->childCount())
		{
			ParentCheckState = Qt::CheckState::Checked;
		}

		SetCheckState(ParentItem, ParentCheckState);
		ParentLogic(ui.treeExisting, ParentItem, ParentCheckState);
	}
}

QCharacterSelectionWindow::QCharacterSelectionWindow(QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);

	ui.treeSelection->setHeaderLabel("diag.txt (static text)");
	ui.treeExisting->setHeaderLabel("Untitled Project");
	//ui.treeExisting->setHeaderHidden(true);

	connect(ui.btnSelection_SelectAll, &QPushButton::pressed, [this]() {
		for (int i = 0; i < this->ui.treeSelection->topLevelItemCount(); i++)
		{
			auto x = this->ui.treeSelection->topLevelItem(i);
			x->setCheckState(0, Qt::CheckState::Checked);
		}
		//this->ui.treeSelection->selectAll();
	});
	connect(ui.btnSelection_DeselectAll, &QPushButton::pressed, [this]() {
		for (int i = 0; i < this->ui.treeSelection->topLevelItemCount(); i++)
		{
			auto x = this->ui.treeSelection->topLevelItem(i);
			x->setCheckState(0, Qt::CheckState::Unchecked);

		//	this->UncheckTreeWidgetItem(this->ui.treeSelection->topLevelItem(i));
		}
		//this->ui.treeSelection->clearSelection();
	});
	connect(ui.btnExisting_SelectAll, &QPushButton::pressed, [this]() {
		return;
		
		for (int i = 0; i < this->ui.treeExisting->topLevelItemCount(); i++)
		{
			auto x = this->ui.treeExisting->topLevelItem(i);
			x->setCheckState(0, Qt::CheckState::Checked);
		}
		//this->ui.treeExisting->selectAll();
	});
	connect(ui.btnExisting_DeselectAll, &QPushButton::pressed, [this]() {
		return;
		
		for (int i = 0; i < this->ui.treeExisting->topLevelItemCount(); i++)
		{
			auto x = this->ui.treeExisting->topLevelItem(i);
			x->setCheckState(0, Qt::CheckState::Unchecked);
		}
		//this->ui.treeExisting->clearSelection();
	});
	connect(ui.treeSelection, &QTreeWidget::itemChanged, [this](QTreeWidgetItem *Item, int Column) {
		
		ProcessCheck(Item, Item->checkState(0));

		//if (Item->parent() == nullptr)
		//{
		//	// Routine for a top-level item.

		//	QTreeWidgetItem *FoundTLItem = nullptr;
		//	QString ItemText = Item->text(0);
		//	int ExistingTopLevelItems = ui.treeExisting->topLevelItemCount();
		//	for (int i = 0; i < ExistingTopLevelItems; i++)
		//	{
		//		auto *CurTLItem = ui.treeExisting->topLevelItem(i);
		//		if (CurTLItem->text(0) == ItemText)
		//		{
		//			FoundTLItem = CurTLItem;
		//			break;
		//		}
		//	}

		//	if (FoundTLItem == nullptr)
		//		return;

		//	QFont ItemFont = FoundTLItem->font(0);
		//	switch (Item->checkState(0))
		//	{
		//	case Qt::CheckState::Checked:
		//		ItemFont.setStrikeOut(true);
		//	case Qt::CheckState::PartiallyChecked:
		//		ItemFont.setItalic(true);
		//		break;
		//	case Qt::CheckState::Unchecked:
		//		ItemFont.setStrikeOut(false);
		//		ItemFont.setItalic(false);
		//		break;
		//	}

		//	FoundTLItem->setFont(0, ItemFont);

		//	// Now iterate through all the top-level's children and set them to the same state.
		//}
		//else
		//{
		//	// Child item
		//	QString ItemText = Item->text(0);
		//	QString ParentText = Item->parent()->text(0);

		//	// Existing side item
		//	QTreeWidgetItem *FoundTLItem = nullptr;
		//	int ExistingTopLevelItems = ui.treeExisting->topLevelItemCount();
		//	for (int i = 0; i < ExistingTopLevelItems; i++)
		//	{
		//		auto *CurTLItem = ui.treeExisting->topLevelItem(i);
		//		auto CurTLItemText = CurTLItem->text(0);
		//		if (CurTLItemText == ParentText)
		//		{
		//			FoundTLItem = CurTLItem;
		//			break;
		//		}
		//	}

		//	if (FoundTLItem != nullptr)
		//	{
		//		QTreeWidgetItem *FoundChildItem = nullptr;
		//		for (int i = 0; i < FoundTLItem->childCount(); i++)
		//		{
		//			FoundChildItem = FoundTLItem->child(i);
		//			auto FoundChildItemText = FoundChildItem->text(0);
		//			if (FoundChildItemText == ItemText)
		//			{
		//				QFont ItemFont = FoundChildItem->font(0);
		//				switch (Item->checkState(0))
		//				{
		//				case Qt::CheckState::Checked:
		//					ItemFont.setStrikeOut(true);
		//					ItemFont.setItalic(true);
		//					break;
		//				case Qt::CheckState::Unchecked:
		//					ItemFont.setStrikeOut(false);
		//					ItemFont.setItalic(false);
		//					break;
		//				}
		//				FoundChildItem->setFont(0, ItemFont);
		//				break;
		//			}
		//		}
		//	}
		//}

		//ui.treeSelection->blockSignals(false);
	});

	QList<QTreeWidgetItem*> Items;
	for (int i = 0; i < 10; i++)
	{
		auto NewItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(QString("Character %1").arg(i)));
		NewItem->setCheckState(0, Qt::CheckState::Unchecked);
		NewItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled /*| Qt::ItemIsAutoTristate*/);

		for (int x = 0; x < 3; x++)
		{
			auto NewChild = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(QString("Diag %1").arg(x)));
			NewChild->setCheckState(0, Qt::Unchecked);
			NewChild->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			NewItem->addChild(NewChild);
		}

		Items.append(NewItem);
	}
	ui.treeSelection->insertTopLevelItems(0, Items);

	Items.clear();
	for (int i = 0; i < 10; i++)
	{
		if (i % 2 == 0) continue;

		auto NewItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(QString("Character %1").arg(i)));
		//NewItem->setCheckState(0, Qt::CheckState::PartiallyChecked);
		NewItem->setFlags(Qt::ItemIsEnabled);

		for (int x = 0; x < 3; x++)
		{
			if (i % 5 == 0 && x <= 1) continue;

			auto NewChild = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(QString("Diag %1").arg(x)));
			//NewChild->setCheckState(0, Qt::Unchecked);
			NewChild->setFlags(Qt::ItemIsEnabled);
			NewItem->addChild(NewChild);
		}

		Items.append(NewItem);
	}
	ui.treeExisting->insertTopLevelItems(0, Items);
}

QCharacterSelectionWindow::~QCharacterSelectionWindow()
{

}

void QCharacterSelectionWindow::UpdateChecks()
{
	//for(auto CurRootIrem : ui.treeSelection->item)
	//ui.treeSelection->item
}