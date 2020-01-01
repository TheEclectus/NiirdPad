#pragma once

#include <stack>
#include <string>

#include <fstream>
#include <sstream>

#include <strtk.hpp>

#include "RawProjectFile.h"

class TUScriptParser
{
public:
	enum class ParserState
	{
		Base,

		Notes,

		Node,
			Dialogue,
			Option
	};

private:
	const std::string _Whitespace = " \t";
	std::stack<ParserState> _parserState;

	RawProjectFile_ScriptFile _outFile;
	RawProjectFile_Node _pendingNode;

	size_t _lineNum = 0u;
	std::string _currentLine = "";
	size_t _cursorPos = 0u;

	std::string _errorMessage = "";

public:
	TUScriptParser();

	void MakeError(const std::string &Message);

	void Parse();

	void AdvanceCursor(size_t Num);
	std::string LineFromCursor();

	bool ConsumeWhitespace();
	void OptConsumeWhitespace();

	bool ConsumeString(const std::string& Str);
	bool ConsumeUntil(const std::string& Str, std::string& Out);
	void ConsumeRemainder(std::string& Out);
	bool ConsumeList(const std::string& Sep, const std::string& End, std::vector<std::string>& Out);

	bool ConsumeNotes();
	bool ConsumeNode();	// Consumes || and any trailing comments
	bool ConsumeDialogue(/*RawProjectFile_Dialogue &Dest*/);
	bool ConsumeOption(/*RawProjectFile_Option &Dest*/);
};