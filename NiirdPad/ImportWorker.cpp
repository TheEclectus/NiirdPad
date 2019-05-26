#include "ImportWorker.h"

#include <fstream>

#include "format.h"

namespace fs = std::experimental::filesystem;

bool ImportWorker::LoadCharacters(const fs::path &CharactersPath)
{
	if (!(fs::is_directory(CharactersPath)))
	{
		_Error = fmt::format("Characters directory '{0}' not found.", CharactersPath.string());
		return false;
	}

	emit SetMessage("Iterating through characters directory...");

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
			_Warnings.push_back(fmt::format("Character '{0}' skipped, no diag.txt found.", CharacterName));
		}
		else
		{
			CharacterPathList.push_back(CurFolder);
		}
	}

	emit SetTotal(CharacterPathList.size());
	emit SetProgress(0);
	int CurCharacterCount = 0;

	for (auto CurCharacterPath : CharacterPathList)
	{
		if (_Cancelled)
		{
			_Error = "Operation cancelled.";
			return false;
		}

		RawProjectFile_Character NewCharacter;
		std::string CharacterName = CurCharacterPath.stem().u8string();

		for (auto CurFile : fs::directory_iterator(CurCharacterPath))
		{
			std::string Filename = CurFile.path().filename().u8string();
			std::string Extension = CurFile.path().extension().u8string();
			if (fs::is_regular_file(CurFile) && Filename.find(u8"diag") == 0u && Extension == u8".txt")
			{
				/*if (Filename == u8"diag_shop")
				{
					_Warnings.push_back(fmt::format("{0}/{1}: File skipped (shop dialogue not yet supported)"));
				}*/

				emit SetMessage(fmt::format("{0} - Parsing {1}...", CharacterName, Filename).c_str());
				//pegtl::file_input<> FileToParse(CurFile.path().u8string());
				std::ifstream InFile{ CurFile.path().u8string() };
				std::string SrcString{ std::istreambuf_iterator<char>(InFile), std::istreambuf_iterator<char>() };
				pegtl::string_input<> FileToParse(SrcString, "");

				TUScript::State ParserState;
				try
				{
					if (!pegtl::parse<TUScript::Grammar, TUScript::Action>(FileToParse, ParserState))
						_Warnings.push_back(fmt::format("Skipped file '{0}' (couldn't parse)", CurFile.path().filename().u8string()));
					else
					{
						RawProjectFile_ScriptFile NewFile(ParserState);
						NewCharacter.AddFile(Filename, NewFile);
					}
				}
				// TODO: I think exceptions are coming from [ | end_encounter] and the like, because of the formatting.
				catch (std::exception Exception)
				{
					size_t Line = FileToParse.line();
					const char *cur = FileToParse.current();
					_Warnings.push_back(fmt::format("{0}/{1}: File skipped (parser error: '{2}')", CharacterName, CurFile.path().filename().u8string(), Exception.what()));
				}
			}
		}

		_LoadedProject.AddCharacter(CharacterName, NewCharacter);

		CurCharacterCount++;
		SetProgress(CurCharacterCount);
	}
}

ImportWorker::ImportWorker(const std::string &TeraurgeDatabasePath) :
	_TeraurgeDatabasePath(fs::absolute(TeraurgeDatabasePath)),
	_Cancelled(false)
{
	
}

ImportWorker::~ImportWorker()
{

}

const std::string &ImportWorker::Error() const
{
	return _Error;
}

const std::vector<std::string> &ImportWorker::Warnings() const
{
	return _Warnings;
}

void ImportWorker::Start()
{
	// Initial state of loading
	emit SetTotal(0);
	emit SetProgress(0);
	emit SetMessage("Scanning directories...");

	fs::path AudioPath = _TeraurgeDatabasePath.u8string() + "\\audio";
	fs::path CardsPath = _TeraurgeDatabasePath.u8string() + "\\cards";
	fs::path CharactersPath = _TeraurgeDatabasePath.u8string() + "\\characters";
	fs::path EnvironmentsPath = _TeraurgeDatabasePath.u8string() + "\\environments";
	fs::path ItemsPath = _TeraurgeDatabasePath.u8string() + "\\items";
	fs::path MapsPath = _TeraurgeDatabasePath.u8string() + "\\maps";
	
	if (!LoadCharacters(CharactersPath))
	{
		emit SendError(_Error);
		emit Finished();
		return;
	}

	//emit SendError(_Error);
	emit SendData(_Warnings, _LoadedProject);
	emit Finished();
}

void ImportWorker::Cancel()
{
	_Cancelled = true;
}