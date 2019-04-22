#include "QImportConsole.h"

#include <filesystem>
#include "format.h"

#include "TUScriptLexer.h"

QImportConsole::QImportConsole(const std::string &RootDirectory, QWidget *parent) : 
	QDialog(parent),
	RootDirectory(RootDirectory)
{
	ui.setupUi(this);
	setWindowFlags(this->windowFlags() ^ Qt::WindowCloseButtonHint);
	connect(ui.btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

QImportConsole::QImportConsole(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags(this->windowFlags() ^ Qt::WindowCloseButtonHint);

	connect(ui.btnClose, &QPushButton::clicked, this, &QDialog::accept);

	AddInfo("Just a test...");
	AddError("Error! You're a brainlet");
}

QImportConsole::~QImportConsole()
{

}

void QImportConsole::showEvent(QShowEvent *event)
{
	QDialog::showEvent(event);

	static bool bAlreadyShown = false;
	if (!bAlreadyShown)
	{
		if (RootDirectory.length() != 0)
			Import();
		bAlreadyShown = true;
	}
}

void QImportConsole::AddInfo(const std::string &Text)
{
	ui.txtConsole->insertPlainText(fmt::format("[INFO] {0}\n", Text).c_str());
}

void QImportConsole::AddError(const std::string &Text)
{
	ui.txtConsole->insertHtml(fmt::format("<p style=\"color:red\">[ERROR] {0}</p>\n", Text).c_str());
	ui.btnClose->setEnabled(true);
}

void QImportConsole::Import()
{
	ui.btnClose->setEnabled(false);

	AddInfo(fmt::format("Attempting to import directory '{0}'...", RootDirectory));

	namespace fs = std::experimental::filesystem;
	fs::path AbsolutePath = fs::absolute(RootDirectory);

	if (!(fs::exists(AbsolutePath) || fs::is_directory(AbsolutePath)))
	{
		AddError(fmt::format("Path '{0}' doesn't exist or is not a directory.", AbsolutePath.string()));
		return;
	}

	// TODO: Check for all the subdirectories of Teraurge's /database folder.
	fs::path CharactersPath = fmt::format("{0}{1}", AbsolutePath.string(), "/characters");
	if (!(fs::exists(CharactersPath) || fs::is_directory(CharactersPath)))
	{
		AddError("Subdirectory '/characters' doesn't exist or is not a directory.");
		return;
	}

	std::vector<fs::path> CharacterFolderPaths;
	for (auto CurCharPath : fs::directory_iterator(CharactersPath))
	{
		bool bFoundDiagTxt = false;
		for (auto CurCharFile : fs::directory_iterator(CurCharPath))
		{
			if (fs::is_regular_file(CurCharFile))
			{
				if (CurCharFile.path().filename() == "diag.txt")
				{
					bFoundDiagTxt = true;
					break;
				}
			}
		}

		if (bFoundDiagTxt)
			CharacterFolderPaths.push_back(CurCharPath);
		else
			AddInfo(fmt::format("Skipping character folder '{0}' ('diag.txt' not found).", CurCharPath.path().string()));
	}
	AddInfo(fmt::format("Found {0} character folders.", CharacterFolderPaths.size()));

	ui.prgsbarSteps->setMaximum(CharacterFolderPaths.size());

	for (auto CurCharPath : CharacterFolderPaths)
	{

		AddInfo(fmt::format("Parsing '{0}'...", CurCharPath.string()));
	}

	ui.btnClose->setEnabled(true);
}