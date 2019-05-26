#include "RawProjectFile.h"

#pragma region RawProjectFile_Dialogue

RawProjectFile_Dialogue::RawProjectFile_Dialogue(const std::string &Reference, const std::vector<std::string> &Scripts, std::string &Dialogue) :
	_Reference(Reference),
	_Scripts(Scripts),
	_Dialogue(Dialogue)
{

}

const std::string &RawProjectFile_Dialogue::Reference() const
{
	return _Reference;
}

const std::vector<std::string> &RawProjectFile_Dialogue::Scripts() const
{
	return _Scripts;
}

const std::string &RawProjectFile_Dialogue::Dialogue() const
{
	return _Dialogue;
}

#pragma endregion

#pragma region RawProjectFile_Option

RawProjectFile_Option::RawProjectFile_Option(const std::string &Pointer, const std::vector<std::string> &VisScripts, const std::vector<std::string> &Functions, const std::string &Text) :
	_Pointer(Pointer),
	_VisibilityScripts(VisScripts),
	_Functions(Functions),
	_Text(Text)
{

}

const std::string &RawProjectFile_Option::Pointer() const
{
	return _Pointer;
}

const std::vector<std::string> &RawProjectFile_Option::VisibilityScripts() const
{
	return _VisibilityScripts;
}

const std::vector<std::string> &RawProjectFile_Option::Functions() const
{
	return _Functions;
}

const std::string &RawProjectFile_Option::Text() const
{
	return _Text;
}

#pragma endregion

const std::string &RawProjectFile_Node::Comment() const
{
	return _Comment;
}

const std::vector<RawProjectFile_Dialogue> &RawProjectFile_Node::Dialogues() const
{
	return _Dialogues;
}

const std::vector<RawProjectFile_Option> &RawProjectFile_Node::Options() const
{
	return _Options;
}

#pragma region RawProjectFile_ScriptFile

RawProjectFile_ScriptFile::RawProjectFile_ScriptFile(TUScript::State &State)
{
	for (auto &CurNode : State.Fragments)
	{
		RawProjectFile_Node NewNode;
		NewNode._Comment = CurNode.Comment;

		for (auto &CurDlg : CurNode.Dialogues)
		{
			NewNode._Dialogues.push_back(RawProjectFile_Dialogue(CurDlg.Reference, CurDlg.Functions, CurDlg.Text));
		}

		for (auto &CurOpt : CurNode.Options)
		{
			NewNode._Options.push_back(RawProjectFile_Option(CurOpt.Pointer, CurOpt.VisibilityScripts, CurOpt.Functions, CurOpt.Text));
		}

		this->_Nodes.push_back(NewNode);
	}
}

const std::vector<RawProjectFile_Node> &RawProjectFile_ScriptFile::Nodes() const
{
	return _Nodes;
}

#pragma endregion


const std::map<std::string, RawProjectFile_ScriptFile> &RawProjectFile_Character::Files() const
{
	return _Files;
}

void RawProjectFile_Character::AddFile(const std::string &FileName, const RawProjectFile_ScriptFile &File)
{
	_Files.insert({ FileName, File });
}

const std::map<std::string, RawProjectFile_Character> &RawProjectFile::Characters() const
{
	return _Characters;
}

void RawProjectFile::AddCharacter(const std::string &CharacterName, const RawProjectFile_Character &Character)
{
	_Characters.insert({ CharacterName, Character });
}