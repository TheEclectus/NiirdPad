#include "ResourceDiscriminator.h"

#include "format.h"

ResourceDiscriminator::Results::Character::Character(const fs::path &Path)
{
	fs::path AbsPath = fs::absolute(Path);

	if (fs::is_directory(AbsPath))
	{
		fs::path::iterator CharacterDirNameIter = AbsPath.end()--;
		std::string CurDirName = CharacterDirNameIter->string();
		while (CurDirName.length() == 0 || CurDirName == ".")
		{
			CharacterDirNameIter--;
			CurDirName = CharacterDirNameIter->string();
		}

		// Get the character name from the current folder.
		this->Name = CharacterDirNameIter->string();

		for (auto CurCharFile : fs::directory_iterator(AbsPath))
		{
			if (fs::is_regular_file(CurCharFile) && fs::file_size(CurCharFile) > 0u)
			{
				// Text files could be notes, credits, or misc.
				if (CurCharFile.path().extension() == ".txt")
				{
					// If it starts with "diag" or contains the word "shop", it's a dialogue file.
					std::string CurFilename = CurCharFile.path().filename().string();
					if (CurFilename.find("diag") == 0u || CurFilename.find("shop") != std::string::npos)
					{
						this->DialogueFiles.push_back(CurCharFile.path().filename());
						continue;
					}
				}
				// PNG files are universally sprites.
				else if (CurCharFile.path().extension() == ".png")
				{
					this->Sprites.push_back(CurCharFile);
					continue;
				}
				// Everything else is a misc file.
				else
				{
					this->MiscFiles.push_back(CurCharFile);
				}
			}
		}

		// If the character has a pictures directory, grab everything from it.
		fs::path PicturesPath = AbsPath / "pictures";
		if (fs::is_directory(PicturesPath))
		{
			for (auto CurCharPic : fs::directory_iterator(PicturesPath))
			{
				if (fs::is_regular_file(CurCharPic) && fs::file_size(CurCharPic) > 0u)
				{
					// All pictures are supposed to be JPGs.
					if (CurCharPic.path().extension() == ".jpg")
						this->Pictures.push_back(CurCharPic);
					// If it's a file but not a jpg, then go ahead and add it as a misc file.
					else
						this->MiscFiles.push_back(CurCharPic);
				}
			}
		}
	}
}

bool ResourceDiscriminator::DatabaseDirectoryIterate()
{
	// Expects a single directory as a target.
	if (_Targets.size() != 1)
	{
		_ErrorMessage = fmt::format("Too many targets described (1 expected, {} received.", _Targets.size());
		return false;
	}

	fs::path DatabasePath = fs::absolute(_Targets.front());
	if (!fs::is_directory(DatabasePath))
	{
		_ErrorMessage = fmt::format("Target '{}' is not a directory.", _Targets.front().string());
		return false;
	}

	//return CharacterDirectoryIterate(DatabasePath) /*| OtherIteratorFunctions()*/;

	CharacterDirectoryIterate(DatabasePath);

	return true;
}

bool ResourceDiscriminator::CharacterDirectoryIterate(fs::path Dir)
{
	fs::path CharactersPath = Dir / "characters";

	if (!fs::is_directory(CharactersPath))
		return false;

	for (auto CurCharDir : fs::directory_iterator(CharactersPath))
	{
		ResourceDiscriminator::Results::Character NewChar(CurCharDir);
		_Results.Characters.insert({ NewChar.Name, NewChar });
	}

	return true;
}

ResourceDiscriminator::ResourceDiscriminator(const std::vector<fs::path> &Targets, ResourceDiscriminator::SelectionType Type) :
	_Targets(Targets),
	_Type(Type)
{
	
}

void ResourceDiscriminator::Start()
{
	if (_Type == ResourceDiscriminator::SelectionType::FullDatabaseFolder)
	{
		if (DatabaseDirectoryIterate())
		{
			//std::sort(_Results.Characters.begin(), _Results.Characters.end());
			emit SendData(_Results, _Warnings);
		}
	}

	emit Finished();
}