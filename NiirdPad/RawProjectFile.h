#pragma once

#include <map>
#include <vector>

#include "TUScriptLexer.h"

class RawProjectFile;
class RawProjectFile_Character;
class RawProjectFile_ScriptFile;
class RawProjectFile_Node;
class RawProjectFile_Dialogue;
class RawProjectFile_Option;

class RawProjectFile_Dialogue
{
private:
	std::string _Reference;
	std::vector<std::string> _Scripts;
	std::string _Dialogue;

public:
	RawProjectFile_Dialogue(const std::string &Reference, const std::vector<std::string> &Scripts, std::string &Dialogue);
	const std::string &Reference() const;
	const std::vector<std::string> &Scripts() const;
	const std::string &Dialogue() const;
};

class RawProjectFile_Option
{
	friend class TUScriptParser;
private:
	std::string _Pointer;
	// VisScripts don't have leading slashes
	std::vector<std::string> _VisibilityScripts, _Functions;
	std::string _Text;

public:
	RawProjectFile_Option(const std::string &Pointer, const std::vector<std::string> &VisScripts, const std::vector<std::string> &Functions, const std::string &Text);
	const std::string &Pointer() const;
	const std::vector<std::string> &VisibilityScripts() const;
	const std::vector<std::string> &Functions() const;
	const std::string &Text() const;
};

class RawProjectFile_Node
{
	friend class RawProjectFile_ScriptFile;
	friend class TUScriptParser;
private:
	std::string _Comment;
	std::vector<RawProjectFile_Dialogue> _Dialogues;
	std::vector<RawProjectFile_Option> _Options;

public:
	const std::string &Comment() const;
	const std::vector<RawProjectFile_Dialogue> &Dialogues() const;
	const std::vector<RawProjectFile_Option> &Options() const;
};

class RawProjectFile_ScriptFile
{
	friend class TUScriptParser;
private:
	std::vector<RawProjectFile_Node> _Nodes;

public:
	RawProjectFile_ScriptFile(TUScript::State &State);
	RawProjectFile_ScriptFile();
	const std::vector<RawProjectFile_Node> &Nodes() const;
};

class RawProjectFile_Character
{
	friend class TUScriptParser;
private:
	std::map<std::string, RawProjectFile_ScriptFile> _Files;

public:
	const std::map<std::string, RawProjectFile_ScriptFile> &Files() const;
	void AddFile(const std::string &FileName, const RawProjectFile_ScriptFile &File);
};

class RawProjectFile
{
private:
	std::map<std::string, RawProjectFile_Character> _Characters;

public:
	const std::map<std::string, RawProjectFile_Character> &Characters() const;
	void AddCharacter(const std::string &CharacterName, const RawProjectFile_Character &Character);
};