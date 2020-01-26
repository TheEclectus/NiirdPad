#include "QCharacterSelectionWindow.h"

QCharacterSelectionWindow::QCharacterSelectionWindow(QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);

	ui.treeSelection->setHeaderLabel("diag.txt (static text)");

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
		}
		//this->ui.treeSelection->clearSelection();
	});
	connect(ui.btnExisting_SelectAll, &QPushButton::pressed, [this]() {
		for (int i = 0; i < this->ui.treeExisting->topLevelItemCount(); i++)
		{
			auto x = this->ui.treeExisting->topLevelItem(i);
			x->setCheckState(0, Qt::CheckState::Checked);
		}
		//this->ui.treeExisting->selectAll();
	});
	connect(ui.btnExisting_DeselectAll, &QPushButton::pressed, [this]() {
		for (int i = 0; i < this->ui.treeExisting->topLevelItemCount(); i++)
		{
			auto x = this->ui.treeExisting->topLevelItem(i);
			x->setCheckState(0, Qt::CheckState::Unchecked);
		}
		//this->ui.treeExisting->clearSelection();
	});
	connect(ui.treeSelection, &QTreeWidget::itemChanged, [this](QTreeWidgetItem *Item, int Column) {
		if (Item->parent() == nullptr)
		{
			// Routine for a top-level item.

			QTreeWidgetItem *FoundTLItem = nullptr;
			QString ItemText = Item->text(0);
			int ExistingTopLevelItems = ui.treeExisting->topLevelItemCount();
			for (int i = 0; i < ExistingTopLevelItems; i++)
			{
				auto *CurTLItem = ui.treeExisting->topLevelItem(i);
				if (CurTLItem->text(0) == ItemText)
				{
					FoundTLItem = CurTLItem;
					break;
				}
			}

			if (FoundTLItem == nullptr)
				return;
			else
			{
				QFont ItemFont = FoundTLItem->font(0);
				switch (Item->checkState(0))
				{
				case Qt::CheckState::Checked:
					ItemFont.setStrikeOut(true);
				case Qt::CheckState::PartiallyChecked:
					ItemFont.setItalic(true);
					break;
				case Qt::CheckState::Unchecked:
					ItemFont.setStrikeOut(false);
					ItemFont.setItalic(false);
					break;
				}

				FoundTLItem->setFont(0, ItemFont);
			}
		}
		else
		{
			// Child item
			QString ItemText = Item->text(0);
			QString ParentText = Item->parent()->text(0);

			// Existing side item
			QTreeWidgetItem *FoundTLItem = nullptr;
			int ExistingTopLevelItems = ui.treeExisting->topLevelItemCount();
			for (int i = 0; i < ExistingTopLevelItems; i++)
			{
				auto *CurTLItem = ui.treeExisting->topLevelItem(i);
				if (CurTLItem->text(0) == ParentText)
				{
					FoundTLItem = CurTLItem;
					break;
				}
			}
		}
	});

	QList<QTreeWidgetItem*> Items;
	for (int i = 0; i < 10; i++)
	{
		auto NewItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(QString("Character %1").arg(i)));
		NewItem->setCheckState(0, Qt::CheckState::PartiallyChecked);
		NewItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);

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