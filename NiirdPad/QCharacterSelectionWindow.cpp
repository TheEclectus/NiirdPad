#include "QCharacterSelectionWindow.h"

#include <QTextBlock>

#include "Character.h"
#include "Project.h"

#include "ResourceDiscriminator.h"

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
	
	connect(ui.treeSelection, &QTreeWidget::itemChanged, [this](QTreeWidgetItem *Item, int Column) {
		ProcessCheck(Item, Item->checkState(0));
	});

	connect(ui.btnAccept, &QPushButton::pressed, [this]() {
		accept();
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

void QCharacterSelectionWindow::closeEvent(QCloseEvent *event)
{
	event->ignore();
	Close();
}

void QCharacterSelectionWindow::ResetForm()
{
	ui.treeSelection->clear();
	ui.treeExisting->clear();
}

void QCharacterSelectionWindow::FormAccepted()
{
	ClearTargets();

	for (int i = 0; i < ui.treeSelection->topLevelItemCount(); i++)
	{
		auto CurItem = ui.treeSelection->topLevelItem(i);
		std::string CurCharName = CurItem->text(0).toStdString();

		for (int x = 0; x < CurItem->childCount(); i++)
		{
			auto CurChild = CurItem->child(i);
		}
	}
}

void QCharacterSelectionWindow::Close()
{
	ResetForm();
	reject();
}

void QCharacterSelectionWindow::ClearTargets()
{
	_targets.clear();
}

void QCharacterSelectionWindow::UpdateChecks()
{
	//for(auto CurRootIrem : ui.treeSelection->item)
	//ui.treeSelection->item
}

void QCharacterSelectionWindow::SelectCharacters(const std::string &CharactersPath, Project *Proj)
{
	if (!Proj)
		return;

	ResetForm();

	//_charactersPath = CharactersPath;
	_existingProject = Proj;

	// Fill the existing side first, might as well.
	for (auto CurChar : _existingProject->Characters())
	{
		std::string CharacterName = CurChar.first;
		QTreeWidgetItem *CharacterItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(CharacterName.c_str()));

		for (auto CurDiag : CurChar.second->DialogueFiles())
		{
			std::string DiagName = CurDiag.first;
			QTreeWidgetItem *DiagItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(DiagName.c_str()));
			CharacterItem->addChild(DiagItem);
		}

		ui.treeExisting->addTopLevelItem(CharacterItem);
	}

	this->show();
}

void QCharacterSelectionWindow::SelectCharacters(const ResourceDiscriminator::Results &Results, Project *Proj)
{
	if (!Proj)
		return;

	ResetForm();

	//_charactersPath = CharactersPath;
	_existingProject = Proj;

	// Fill the existing side first, might as well.
	for (auto CurChar : _existingProject->Characters())
	{
		std::string CharacterName = CurChar.first;
		QTreeWidgetItem *CharacterItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(CharacterName.c_str()));

		for (auto CurDiag : CurChar.second->DialogueFiles())
		{
			std::string DiagName = CurDiag.first;
			QTreeWidgetItem *DiagItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(DiagName.c_str()));
			CharacterItem->addChild(DiagItem);
		}

		ui.treeExisting->addTopLevelItem(CharacterItem);
	}

	// Now the import side.
	for (auto CurChar : Results.Characters)
	{
		std::string CharacterName = CurChar.first;
		QTreeWidgetItem *CharacterItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(CharacterName.c_str()));

		for (auto CurDiag : CurChar.second.DialogueFiles)
		{
			std::string DiagName = CurDiag.string();
			QTreeWidgetItem *DiagItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(DiagName.c_str()));
			CharacterItem->addChild(DiagItem);
		}

		ui.treeSelection->addTopLevelItem(CharacterItem);
	}

	for (int i = 0; i < this->ui.treeSelection->topLevelItemCount(); i++)
	{
		auto x = this->ui.treeSelection->topLevelItem(i);
		x->setCheckState(0, Qt::CheckState::Checked);
	}

	this->show();
}