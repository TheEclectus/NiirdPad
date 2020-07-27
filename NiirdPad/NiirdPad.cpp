#include "NiirdPad.h"

#include <filesystem>

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QThread>
#include <QVariant>

#include "format.h"
//#include "QImportConsole.h"
#include "ImportWorker.h"
#include "QReferenceEditWindow.h"
#include "QScriptEditWindow.h"
#include "ReferenceDatabase.h"

#include "Character.h"
#include "DialogueFile.h"
#include "Node.h"

#include "TUScriptLexer_Exp.h"

void NiirdPad::Import()
{
	QFileDialog Dialog;
	Dialog.setWindowTitle("Select /Teraurge/database/ folder...");
	Dialog.setFileMode(QFileDialog::Directory);
	Dialog.setOption(QFileDialog::ShowDirsOnly);

	if (Dialog.exec() != QDialog::Accepted)
		return;
	
	/*namespace fs = std::experimental::filesystem;

	fs::path AbsoluteImportPath = fs::absolute(Dialog.directory().absolutePath().toStdString());
	QProgressDialog Prg(fmt::format("Importing {}", AbsoluteImportPath.string()).c_str(), "Cancel", 0, 0, this);
	Prg.setWindowModality(Qt::WindowModality::ApplicationModal);
	Prg.resize(600, Prg.height());
	Prg.exec();

	while (true)
	{
		//QApplication::processEvents();

		if (Prg.wasCanceled())
			break;
	}*/

	//std::atomic<bool> bThreadFinished = false;
	QThread *ImportThread = new QThread(this);
	ImportWorker *Worker = new ImportWorker(Dialog.directory().absolutePath().toStdString());
	QProgressDialog *Prg = new QProgressDialog("", "Cancel", 0, 0, this);
	//QMessageBox *Msg = new QMessageBox(this);
	Prg->resize(300, Prg->height());
	Prg->setWindowTitle(fmt::format("Importing '{0}'", Dialog.directory().absolutePath().toStdString()).c_str());
	Prg->setWindowModality(Qt::WindowModality::ApplicationModal);
	Prg->setModal(true);
	Prg->show();

	Worker->moveToThread(ImportThread);

	connect(ImportThread, &QThread::started, Worker, &ImportWorker::Start);
	connect(Worker, &ImportWorker::SendError, [&](const std::string &Error)
	{
		if(Error != "")
			fmt::print("Encountered an error: {}.\n", Error);
	});
	//connect(Worker, &ImportWorker::SendData, [&](std::vector<std::string> Warnings, RawProjectFile Files)
	//{
	//	if (Warnings.size() > 0)
	//	{
	//		auto Msg = new QMessageBox(this); // this->_importMessageBox;

	//		QString WarningString = fmt::format("== {0} Warnings: ==\n", Warnings.size()).c_str();
	//		for (auto a : Warnings)
	//		{
	//			WarningString.append(a.c_str());
	//			WarningString.append("\n");
	//		}
	//		//QMessageBox *Msg = new QMessageBox(this);
	//		Msg->setWindowTitle("Warnings");
	//		Msg->setText("Warnings were generated during import. Continue?");
	//		Msg->setIcon(QMessageBox::Icon::Warning);
	//		Msg->setText(WarningString);
	//		Msg->setStandardButtons({ QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel });
	//		Msg->exec();
	//	}
	//});
	connect(Worker, &ImportWorker::SendData, this, &NiirdPad::ImportConfirmationMessageBox);

	connect(Worker, &ImportWorker::Finished, ImportThread, &QThread::quit);
	connect(Worker, &ImportWorker::Finished, Worker, &ImportWorker::deleteLater);
	connect(Worker, &ImportWorker::Finished, Prg, &QProgressDialog::deleteLater);

	connect(Worker, &ImportWorker::SetTotal, Prg, &QProgressDialog::setMaximum);
	connect(Worker, &ImportWorker::SetProgress, Prg, &QProgressDialog::setValue);
	connect(Worker, &ImportWorker::SetMessage, Prg, &QProgressDialog::setLabelText);
	
	// TODO: Cancel button currently doesn't do anything.

	/*connect(Prg, &QProgressDialog::cancel, ImportThread, &QThread::quit);
	connect(Prg, &QProgressDialog::cancel, Worker, &ImportWorker::deleteLater);
	connect(Prg, &QProgressDialog::cancel, Prg, &QProgressBar::deleteLater);*/

	ImportThread->start();

	/*while (!bThreadFinished)
	{

	}

	if (Worker->Error().length() > 0)
	{
		QMessageBox *Msg = new QMessageBox(QMessageBox::Icon::Warning, "Error", "TUScript parser has encountered an error.", QMessageBox::StandardButton::Ok, this);
		Msg->setDetailedText(Worker->Error().c_str());
		Msg->exec();
		return;
	}

	if (Worker->Warnings().size() > 0)
	{
		QMessageBox *Msg = new QMessageBox(QMessageBox::Icon::Warning, "Warning", fmt::format("TUScript parser has encountered {0} warnings. Proceed?", Worker->Warnings().size()).c_str(), { QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No }, this);
		QString WarningsStr;
		for (const std::string &Warning : Worker->Warnings())
		{
			WarningsStr.append(fmt::format("{0}\n", Warning.c_str()).c_str());
		}
		Msg->setDetailedText(Worker->Error().c_str());
		Msg->exec();
		return;
	}*/
}

bool NiirdPad::ImportExperimental()
{
	QFileDialog Dialog;
	Dialog.setWindowTitle("Select /Teraurge/database/ folder...");
	Dialog.setFileMode(QFileDialog::Directory);
	Dialog.setOption(QFileDialog::ShowDirsOnly);

	if (Dialog.exec() != QDialog::Accepted)
		return false;

	RawProjectFile LoadedProject;
	std::string CharactersPath = Dialog.directory().absolutePath().toStdString();
	std::vector<std::string> Warnings;
	std::string Error = "";

	namespace fs = std::experimental::filesystem;
	if (!(fs::is_directory(CharactersPath)))
	{
		Error = fmt::format("Characters directory '{0}' not found.", CharactersPath);
		return false;
	}

	// TODO: Check for and load from character_list.txt
	std::vector<fs::path> CharacterPathList;
	for (auto CurFolder : fs::directory_iterator(CharactersPath))
	{
		if (!fs::is_directory(CurFolder))
			continue;

		std::string CharacterName = CurFolder.path().stem().u8string();

		// Skip template_ folders
		if (CharacterName.find(u8"template_") == 0u)
			continue;

		bool bDirectoryHadDiagTxt = false;
		for (auto CurFile : fs::directory_iterator(CurFolder))
		{
			if (fs::is_regular_file(CurFile) && CurFile.path().filename() == "diag.txt")
			{
				bDirectoryHadDiagTxt = true;
				break;
			}
		}

		if (!bDirectoryHadDiagTxt)
		{
			Warnings.push_back(fmt::format("Character '{0}' skipped, no diag.txt found.", CharacterName));
		}
		else
		{
			CharacterPathList.push_back(CurFolder);
		}
	}

	int CurCharacterCount = 0;
	for (auto CurCharacterPath : CharacterPathList)
	{
		/*if (_Cancelled)
		{
			_Error = "Operation cancelled.";
			return false;
		}*/

		RawProjectFile_Character NewCharacter;
		std::string CharacterName = CurCharacterPath.stem().u8string();

		for (auto CurFile : fs::directory_iterator(CurCharacterPath))
		{
			std::string Filename = CurFile.path().filename().u8string();
			std::string Extension = CurFile.path().extension().u8string();
			if (fs::is_regular_file(CurFile) && Filename.find(u8"diag") == 0u && Extension == u8".txt")
			{
				TUScriptParser PS(CurFile.path().u8string());
				PS.Parse();

				if (PS.GetError().length() > 0)
				{
					Warnings.push_back(fmt::format("{0}/{1}: File skipped (parser error: '{2}')", CharacterName, CurFile.path().filename().u8string(), PS.GetError()));
				}
				else
				{
					NewCharacter.AddFile(Filename, PS.GetScriptFile());
				}
			}
		}

		LoadedProject.AddCharacter(CharacterName, NewCharacter);

		CurCharacterCount++;
		//SetProgress(CurCharacterCount);
	}

	ImportConfirmationMessageBox(Warnings, LoadedProject);
}

void NiirdPad::ResetCharacterCombo()
{
	ui.cmbCharacter->clear();

	if (_loadedProject)
	{
		for (auto &CurChar : _loadedProject->Characters())
		{
			ui.cmbCharacter->addItem(CurChar.first.c_str(), QVariant::fromValue((void*)CurChar.second));
		}
	}

	ResetDialogueFileCombo();
}

void NiirdPad::ResetDialogueFileCombo()
{
	ui.cmbDiag->clear();

	if (_loadedProject && ui.cmbCharacter->count() > 0)
	{
		Character *SelectedChar = (Character*)ui.cmbCharacter->currentData().value<void*>();

		for (auto &CurDiag : SelectedChar->DialogueFiles())
		{
			ui.cmbDiag->addItem(CurDiag.first.c_str(), QVariant::fromValue((void*)CurDiag.second));
		}
	}

	ResetIndexCombo();
}

void NiirdPad::ResetIndexCombo()
{
	ui.cmbJump->clear();

	if (_loadedProject && ui.cmbCharacter->count() > 0 && ui.cmbDiag->count() > 0)
	{
		std::vector<std::pair<std::string, void*>> ListItems;
		DialogueFile *SelectedDiag = (DialogueFile*)ui.cmbDiag->currentData().value<void*>();

		for (auto &CurNode : SelectedDiag->GetNodes())
		{
			for (auto CurDlg : CurNode->Dialogues())
			{
				//ui.cmbJump->addItem(CurDlg->GetReference().c_str(), QVariant::fromValue((void*)CurDlg));
				ListItems.push_back({ CurDlg->GetReference(), (void*)CurDlg });
			}
		}

		std::sort(ListItems.begin(), ListItems.end());

		for (auto CurPair : ListItems)
		{
			ui.cmbJump->addItem(CurPair.first.c_str(), QVariant::fromValue(CurPair.second));
		}
	}

	if (ui.cmbJump->count() > 0)
	{
		ui.btnJump->setEnabled(true);
	}
	else
	{
		ui.btnJump->setEnabled(false);
	}
}

void NiirdPad::SetWindowTitle()
{
	std::string WindowTitle = "NiirdPad v" NIIRDPAD_VERSION " - build " NIIRDPAD_BUILD_ID " - ";
	if (_loadedProject->SavePath() != "")
		WindowTitle += QFile(_loadedProject->SavePath().c_str()).fileName().toStdString();
	else
		WindowTitle += "Untitled Project";

	if (_loadedProject->UnsavedChanges())
		WindowTitle += "*";

	this->setWindowTitle(WindowTitle.c_str());
}

void NiirdPad::closeEvent(QCloseEvent *Event)
{
	if (_loadedProject->UnsavedChanges())
	{
		QMessageBox::StandardButton Res = QMessageBox::warning(this, "Project Has Unsaved Changes", "Do you want to save changes to the current project?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (Res == QMessageBox::StandardButton::Save)
		{
			if (_loadedProject->Save())
				Event->accept();
			else
				Event->ignore();
		}
		else if (Res == QMessageBox::StandardButton::Discard)
		{
			Event->accept();
		}
		else
		{
			Event->ignore();
		}
	}
	else
		Event->accept();
}

void NiirdPad::ImportConfirmationMessageBox(std::vector<std::string> Warnings, RawProjectFile Files)
{
	if (Warnings.size() > 0)
	{
		QMessageBox Msg(this);
		QString WarningString = ""; //fmt::format("== {0} Warnings: ==\n", Warnings.size()).c_str();
		for (auto a : Warnings)
		{
			WarningString.append(a.c_str());
			WarningString.append("\n\n");
		}
		//QMessageBox *Msg = new QMessageBox(this);
		Msg.setWindowTitle(fmt::format("{0} Warnings", Warnings.size()).c_str());
		Msg.setText("Warnings were generated during import. Continue?\nConnecting the imported Nodes will take some time, and the program may appear to freeze during the process.");
		Msg.setIcon(QMessageBox::Icon::Warning);
		Msg.setDetailedText(WarningString);
		Msg.setStandardButtons({ QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel });
		if (Msg.exec() == QMessageBox::Cancel)
			return;
	}
	else
	{
		QMessageBox Msg(this);
		//QMessageBox *Msg = new QMessageBox(this);
		Msg.setWindowTitle("Connect Nodes");
		Msg.setText("Imported Nodes will now be connected. Connecting the imported nodes will take some time, and the program may appear to freeze during the process.");
		Msg.setIcon(QMessageBox::Icon::Warning);
		Msg.setStandardButtons({ QMessageBox::StandardButton::Ok });
		if (Msg.exec() == QMessageBox::Cancel)
			return;
	}

	// TODO: Do the RawProjectFile -> ProjectFile conversion here
	std::string ErrorMsg = "";
	if (!_loadedProject->Load(Files, &ErrorMsg))
	{
		QMessageBox::critical(this, "Failed To Load Project", std::string("Error: '" + ErrorMsg + "'").c_str(), QMessageBox::Ok);
		return;
	}
	SetWindowTitle();
	ResetCharacterCombo();

	QMessageBox PostMsg(this);
	//QMessageBox *Msg = new QMessageBox(this);
	PostMsg.setWindowTitle(fmt::format("Project Imported{}", ErrorMsg.length() > 0 ? " With Warnings" : "").c_str());
	PostMsg.setText(fmt::format("Project successfully imported{}", ErrorMsg.length() > 0 ? ". See below for more details." : " with no warnings.").c_str());
	PostMsg.setIcon(QMessageBox::Icon::Information);
	PostMsg.setDetailedText(ErrorMsg.c_str());
	PostMsg.setStandardButtons({ QMessageBox::StandardButton::Ok });
	PostMsg.exec();
}

void NiirdPad::ResourceSelectionFollowup(ResourceDiscriminator::Results Results, std::vector<std::string> Warnings)
{
	_characterSelectionWindow->SelectCharacters(Results, _loadedProject);
}

NiirdPad::NiirdPad(QWidget *parent) :
	QMainWindow(parent),
	_scriptEngine(),
	_characterWindow(new QCharacterWindow(this)),
	_characterSelectionWindow(new QCharacterSelectionWindow(this)),
	_commentEditWindow(new QCommentEditWindow(this)),
	_scriptEditWindow(new QScriptEditWindow(this, _scriptEngine, false)),
	_referenceEditWindow(new QReferenceEditWindow(this)),
	_dialogueFileWindow(new QDialogueFileWindow(this))
{
	ui.setupUi(this);

	// HACK: hack attack, whack your shmack and clack your knacks :^)
	ui.widget->SetEngine(&_scriptEngine);
	ui.widget->SetNiirdPad(this);
	ui.widget->ConnectToReferenceEditWindow();

	connect(ui.actionExperimental_TUScript_Parser, &QAction::triggered, [this]() {
		this->ImportExperimental();
	});

	connect(ui.actionCharacter_Selection_Window, &QAction::triggered, [this]() {
		//_characterSelectionWindow->show();

		QFileDialog Dialog;
		Dialog.setWindowTitle("Select /Teraurge/database/ folder...");
		Dialog.setFileMode(QFileDialog::Directory);
		Dialog.setOption(QFileDialog::ShowDirsOnly);

		if (Dialog.exec() != QDialog::Accepted)
			return;

		QThread *ResourceWorkerThread = new QThread(this);
		ResourceDiscriminator *ResDisc = new ResourceDiscriminator({ Dialog.directory().absolutePath().toStdString() }, ResourceDiscriminator::SelectionType::FullDatabaseFolder);
		QProgressDialog *Prg = new QProgressDialog("", "Cancel", 0, 0, this);
		Prg->resize(300, Prg->height());
		Prg->setWindowTitle(fmt::format("Reading '{0}'", Dialog.directory().absolutePath().toStdString()).c_str());
		Prg->setWindowModality(Qt::WindowModality::ApplicationModal);
		Prg->setModal(true);
		Prg->show();

		ResDisc->moveToThread(ResourceWorkerThread);

		connect(ResourceWorkerThread, &QThread::started, ResDisc, &ResourceDiscriminator::Start);

		connect(ResDisc, &ResourceDiscriminator::SendData, this, &NiirdPad::ResourceSelectionFollowup);

		connect(ResDisc, &ResourceDiscriminator::Finished, ResourceWorkerThread, &QThread::quit);
		connect(ResDisc, &ResourceDiscriminator::Finished, ResDisc, &ResourceDiscriminator::deleteLater);
		connect(ResDisc, &ResourceDiscriminator::Finished, Prg, &QProgressDialog::deleteLater);
		//connect(ResDisc, &ResourceDiscriminator::Finished, [this, ResourceWorkerThread, ResDisc, Prg](ResourceDiscriminator::Results Results, std::vector<std::string> Warnings) {
		//	ResourceWorkerThread->quit();
		//	ResDisc->deleteLater();
		//	Prg->deleteLater();

		//	//this->_characterSelectionWindow->SelectCharacters(Results, this->_loadedProject);
		//});
		

		ResourceWorkerThread->start();

		//_characterSelectionWindow->SelectCharacters(Dialog.directory().absolutePath().toStdString(), this->_loadedProject);
	});

	// Adding a character
	connect(_characterWindow, &QCharacterWindow::NameAdded, [this](std::string NewName) {
		this->ResetCharacterCombo();

		ui.cmbCharacter->setCurrentIndex(ui.cmbCharacter->findText(NewName.c_str()));
		auto VB = this->ui.widget->GetCamera().ViewBox;
		this->ui.widget->GetCamera().ViewBox = { 0, 0, VB.w, VB.h };
	});

	// Editing a character's name
	connect(_characterWindow, &QCharacterWindow::NameEdited, [this](std::string NewName) {
		this->ResetCharacterCombo();

		ui.cmbCharacter->setCurrentIndex(ui.cmbCharacter->findText(NewName.c_str()));
	});

	// Adding a dialogue file to a character
	connect(_dialogueFileWindow, &QDialogueFileWindow::FileAdded, [this](std::string NewFile) {
		this->ResetDialogueFileCombo();

		ui.cmbDiag->setCurrentIndex(ui.cmbDiag->findText(NewFile.c_str()));
		auto VB = this->ui.widget->GetCamera().ViewBox;
		this->ui.widget->GetCamera().ViewBox = { 0, 0, VB.w, VB.h };
	});

	// Editing a character's dialogue file's name
	connect(_dialogueFileWindow, &QDialogueFileWindow::FileEdited, [this](std::string NewFile) {
		this->ResetDialogueFileCombo();

		ui.cmbDiag->setCurrentIndex(ui.cmbDiag->findText(NewFile.c_str()));
	});

	/*
		Character buttons
		- Add
		- Rename
		- Remove
	*/
	connect(ui.btnAddCharacter, &QPushButton::pressed, [this]() {
		this->_characterWindow->NewCharacter(this->_loadedProject);
	});

	connect(ui.btnRenameCharacter, &QPushButton::pressed, [this]() {
		this->_characterWindow->EditCharacter(this->_loadedProject, (Character*)ui.cmbCharacter->currentData().value<void*>());
	});

	connect(ui.btnDeleteCharacter, &QPushButton::pressed, [this]() {
		if (ui.cmbCharacter->count() > 1)
		{
			if (QMessageBox::warning(this, "Warning", "Deleting a character cannot be undone. Continue?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel) != QMessageBox::Yes)
				return;

			_loadedProject->DeleteCharacter((Character*)ui.cmbCharacter->currentData().value<void*>());
			ResetCharacterCombo();
		}
		else
		{
			QMessageBox::warning(this, "Cannot Delete Character", "Cannot delete final character.");
		}
	});

	/*
		Dialogue buttons
		- Add
		- Rename
		- Remove
	*/
	connect(ui.btnAddDiagFile, &QPushButton::pressed, [this]() {
		//this->_characterWindow->NewCharacter(this->_loadedProject);
		Character *Char = (Character*)ui.cmbCharacter->currentData().value<void*>();
		this->_dialogueFileWindow->NewDialogueFile(Char);
	});

	connect(ui.btnRenameDiagFile, &QPushButton::pressed, [this]() {
		//this->_characterWindow->EditCharacter(this->_loadedProject, (Character*)ui.cmbCharacter->currentData().value<void*>());

		Character *Char = (Character*)ui.cmbCharacter->currentData().value<void*>();
		DialogueFile *Diag = (DialogueFile*)ui.cmbDiag->currentData().value<void*>();

		this->_dialogueFileWindow->EditDialogueFile(Char, Diag);
	});

	connect(ui.btnDeleteDiagFile, &QPushButton::pressed, [this]() {
		if (ui.cmbDiag->count() > 1)
		{
			if (QMessageBox::warning(this, "Warning", "Deleting a dialogue file cannot be undone. Continue?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel) != QMessageBox::Yes)
				return;

			_loadedProject->DeleteCharacter((Character*)ui.cmbDiag->currentData().value<void*>());
			ResetCharacterCombo();
		}
		else
		{
			QMessageBox::warning(this, "Cannot Delete Dialogue File", "Cannot delete final dialogue file.");
		}
	});

	connect(ui.actionImportProject, &QAction::triggered, [this]() {
		auto Confirmation = QMessageBox::information(this, "WIP Feature", "Project importing is still a WIP feature, and errors are to be expected. Currently there is no automatic layout algorithm, and Nodes must be arranged by hand. Erroneous Options will not be connected. Continue?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
		if (Confirmation != QMessageBox::Yes)
			return;

		if (_loadedProject->UnsavedChanges())
		{
			QMessageBox::StandardButton Res = QMessageBox::warning(this, "Project Has Unsaved Changes", "Do you want to save changes to the current project?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			if (Res == QMessageBox::StandardButton::Save)
			{
				if (!_loadedProject->Save())
					return;
			}
			else if (Res == QMessageBox::StandardButton::Cancel)
			{
				return;
			}
		}
		
		this->Import();

		/*QFileDialog Dialog;
		Dialog.setWindowTitle("Select /Teraurge/database/ folder...");
		Dialog.setFileMode(QFileDialog::Directory);
		Dialog.setOption(QFileDialog::ShowDirsOnly);

		if (Dialog.exec() == QDialog::Accepted)
		{
			QImportConsole Con(Dialog.directory().absolutePath().toStdString());
			Con.exec();
		}*/
	});

	connect(ui.actionNew_Project, &QAction::triggered, [this]() {
		if (_loadedProject->UnsavedChanges())
		{
			QMessageBox::StandardButton Res = QMessageBox::warning(this, "Project Has Unsaved Changes", "Do you want to save changes to the current project?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			if (Res == QMessageBox::StandardButton::Save)
			{
				if (!_loadedProject->Save())
					return;
			}
			else if (Res == QMessageBox::StandardButton::Cancel)
			{
				return;
			}
		}
		_loadedProject->New();
		SetWindowTitle();
		ResetCharacterCombo();
	});

	connect(ui.actionSave, &QAction::triggered, [this]() {
		if(this->_loadedProject->Save())
			SetWindowTitle();
	});

	connect(ui.actionSave_As, &QAction::triggered, [this]() {
		if(this->_loadedProject->SaveAs(""))
			SetWindowTitle();
	});

	connect(ui.actionOpen, &QAction::triggered, [this]() {
		if (_loadedProject->UnsavedChanges())
		{
			QMessageBox::StandardButton Res = QMessageBox::warning(this, "Project Has Unsaved Changes", "Do you want to save changes to the current project?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			if (Res == QMessageBox::StandardButton::Save)
			{
				if (!_loadedProject->Save())
					return;
			}
			else if (Res == QMessageBox::StandardButton::Cancel)
			{
				return;
			}
		}
		
		std::string ErrorMsg = "";
		if (!this->_loadedProject->Load("", &ErrorMsg))
		{
			QMessageBox::critical(this, "Failed To Load Project", std::string("Error: '" + ErrorMsg + "'").c_str(), QMessageBox::Ok);
			return;
		}
		else
		{
			SetWindowTitle();
			ResetCharacterCombo();
		}
	});

	connect(ui.actionEditReferenceWindowNew, &QAction::triggered, [this]() {
		std::string Res = "";
		QReferenceEditWindow::NewReference(this, Res);
	});
	connect(ui.actionEditReferenceWindowEdit, &QAction::triggered, [this]() {
		std::string Res = "";
		QReferenceEditWindow::EditReference(this, Res);
	});
	connect(ui.actionDump_Reference_Database, &QAction::triggered, [this]() {
		fmt::print("====== REFERENCE DUMP ======\n");
		for (auto i : this->ui.widget->GetDialogueFile()->GetReferenceDatabase().Map())
		{
			fmt::print("{} - {}\n", i.first, fmt::ptr(i.second));
		}
		fmt::print("============================\n");
	});
	connect(ui.actionImportConsole, &QAction::triggered, [this]() {
		//QImportConsole Con(this);
		//Con.exec();
	});

	// Neither option actually has a NodeDialogue or NodeOption to edit by design. No fucking around with needing the SDL state.
	connect(ui.actionEditScriptWindowDialogue, &QAction::triggered, [this]() {
		QScriptEditWindow Win(this->window(), this->_scriptEngine, true);
		Win.exec();
	});
	connect(ui.actionEditScriptWindowOption, &QAction::triggered, [this]() {
		QScriptEditWindow Win(this, this->_scriptEngine, false);
		Win.exec();
	});

	connect(ui.cmbCharacter, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
		this->ResetDialogueFileCombo();
	});
	connect(ui.cmbDiag, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
		DialogueFile *DiagFile = (DialogueFile*)ui.cmbDiag->itemData(index).value<void*>();
		this->ui.widget->SetDialogueFile(DiagFile);
		this->ResetIndexCombo();
	});
	connect(ui.cmbCharacter, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
		//NodeDialogue *Dlg = (NodeDialogue*)ui.cmbDiag->itemData(index).value<void*>();
		//this->ui.widget->JumpTo(Dlg);
	});
	connect(ui.btnJump, &QPushButton::pressed, [this]() {
		NodeDialogue *Dlg = (NodeDialogue*)ui.cmbJump->currentData().value<void*>();
		this->ui.widget->JumpTo(Dlg);
	});

	// TODO: Hide debug menu and console when not launched with --debug flag
	//ui.menuDebug->menuAction()->setVisible(false);

	//ui.widget->setFocus();
	//this->setMouseTracking(true);

	_loadedProject = new Project(*ui.widget);
	_loadedProject->New();

	//Character *NewChar = _loadedProject->NewCharacter("NIIRB"); // Legally distinct from Niirds™

	//DialogueFile *NewDiag = NewChar->NewDialogueFile("diag");
	//Node *NewNode = NewDiag->NewNode();
	//NewNode->SetComment("Presenting, the Niirb");

	//auto *Dlg = NewNode->AddDialogue("b i r b");
	//Dlg->SetAll({ "give_money \"krats\" 20" }, "the niirb is a legally distinct creature from the Niird, which is copyright 2016-2019 Meandraco Entertainment");
	//auto *Opt = NewNode->AddOption();
	//Opt->SetAll({ "//showif.has_stat.charisma.8" }, { "give_money krats 80" }, "Test");
	//Opt->Nub().Connections()[0]->Connect(&Dlg->Nub().Connection());

	//auto *Opt2 = NewNode->AddOption();
	//Opt2->SetAll({ "//showif.charisma.8" }, { "day_night_gate" }, "Test 2: Electric Boogaloo");

	//DialogueFile *NewDiag2 = NewChar->NewDialogueFile("diag 2");
	//Node *NewNode2 = NewDiag2->NewNode();
	//NewNode2->SetComment("Presenting, the Niirb 2");

	//auto *Dlg2 = NewNode2->AddDialogue("b i r b 2");
	//Dlg2->SetAll({ "give_money \"krats\" 80" }, "plz no sue");

	ResetCharacterCombo();
	SetWindowTitle();

	//ui.widget->SetDialogueFile(NewDiag);
}

QCommentEditWindow *NiirdPad::CommentEditWindow()
{
	return _commentEditWindow;
}

QScriptEditWindow *NiirdPad::ScriptEditWindow()
{
	return _scriptEditWindow;
}

QReferenceEditWindow *NiirdPad::ReferenceEditWindow()
{
	return _referenceEditWindow;
}

ScriptEngine &NiirdPad::ScriptEngine()
{
	return _scriptEngine;
}

void NiirdPad::DirtyProjectChanges()
{
	_loadedProject->DirtyChanges();
	SetWindowTitle();
}

void NiirdPad::CleanProjectChanges()
{
	_loadedProject->CleanChanges();
	SetWindowTitle();
}

Project *NiirdPad::LoadedProject()
{
	return _loadedProject;
}