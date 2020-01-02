#include "TUScriptLexer_Exp.h"

#include <vector>

#include "format.h"

std::string TestDiag = R"(NOTES
This is where some random-ass notes gonna go
					wheeeeeeeeeeee I'm over here
   now I'm back over here whatcha gonna do
NOTES

|| >> test comment
{start | hide_character, counter falmilen_anger + 0} You are standing at the entrance of a cave. If you listen closely, whistling echoing sounds can occasionally be heard from the inside.
	[test_option] And here's an option.
||)";

//std::string TestDiag = R"(NOTES
//This is where some random-ass notes gonna go
//					wheeeeeeeeeeee I'm over here
//   now I'm back over here whatcha gonna do
//NOTES)";

TUScriptParser::TUScriptParser(const std::string &FilePath)
{
	strtk::for_each_line(FilePath, [this](const std::string &Line) {
		std::string NewLine = Line;
		strtk::remove_trailing(_Whitespace, NewLine);

		//bool hold = NewLine == "||>>witch talks";

		//std::string NewLineNoWhitespace = NewLine;
		//strtk::remove_leading_trailing(_Whitespace, NewLineNoWhitespace);
		//if(NewLineNoWhitespace.length() > 0)
		this->_scriptLines.push_back(NewLine);
	});

	//strtk::remove_empty_strings(_scriptLines);
}

void TUScriptParser::MakeError(const std::string &Message)
{
	if (_errorMessage.length() > 0)
		_errorMessage += "\n";
	_errorMessage = fmt::format("{}:{}: {}", _lineNum + 1, _cursorPos, Message);
}

const std::string & TUScriptParser::GetError()
{
	return _errorMessage;
}

void TUScriptParser::Parse()
{
	_parserState.push(ParserState::Base);

	while (_lineNum <= _scriptLines.size() - 1)
	{
		_currentLine = _scriptLines[_lineNum];
		_cursorPos = 0u;

		std::string CurrentLineCopyNoWhitespace = _currentLine;
		strtk::remove_leading_trailing(_Whitespace, CurrentLineCopyNoWhitespace);
		if (CurrentLineCopyNoWhitespace.length() == 0)
		{
			_lineNum++;
			continue;
		}

		switch (_parserState.top())
		{
		case ParserState::Base:
			if (_currentLine.find("NOTES") == 0u)
			{
				if (!ConsumeString("NOTES"))
					return;

				OptConsumeWhitespace();

				_parserState.push(ParserState::Notes);
			}
			else if (_currentLine.find("||") == 0u)
			{
				if (!ConsumeNode())
					return;

				_parserState.push(ParserState::Node);
			}
			else
			{
				MakeError("Invalid root definition.");
				return;
			}
			break;

		case ParserState::Notes:
			if (_currentLine.find("NOTES") == 0u)
			{
				if (!ConsumeString("NOTES"))
					return;

				OptConsumeWhitespace();

				_parserState.pop();
			}
			else
			{
				if (!ConsumeNotes())
					return;
			}
			break;

		case ParserState::Node:
			if (_currentLine.find("||") == 0u)
			{
				if (/*_pendingNode.Comment().length() != 0 ||*/ _pendingNode.Dialogues().size() != 0 || _pendingNode.Options().size() != 0)
				{
					_outFile._Nodes.push_back(_pendingNode);
					_pendingNode = RawProjectFile_Node();
				}

				if (!ConsumeNode())
					return;
			}
			else if (_currentLine.find("NOTES") == 0u)
			{
				if (!ConsumeString("NOTES"))
					return;

				OptConsumeWhitespace();

				_parserState.push(ParserState::Notes);
			}
			else if (_currentLine.find('{') == 0u)
			{
				if (_parserState.top() == ParserState::Option)
				{
					MakeError("Dialogue defined after final Option definition.");
					return;
				}

				if (!ConsumeDialogue())
					return;
			}
			else if (_currentLine.find('\t') == 0u)
			{
				if (!ConsumeOption())
					return;
			}
			break;
		}


		if (_currentLine.length() > 0)
		{
			MakeError("Unconsumed characters before end-of-line.");
			return;
		}
		_lineNum++;
	}

	if (_parserState.top() == ParserState::Node && (/*_pendingNode.Comment().length() != 0 ||*/ _pendingNode.Dialogues().size() != 0 || _pendingNode.Options().size() != 0))
	{
		MakeError("Missing '||' after Node declaration.");
	}
	else if (_parserState.top() == ParserState::Notes)
	{
		MakeError("Unclosed NOTES section.");
	}
}

void TUScriptParser::AdvanceCursor(size_t Num)
{
	_cursorPos += Num;
	_currentLine = _currentLine.substr(Num);
}

std::string TUScriptParser::LineFromCursor()
{
	if (_currentLine.size() == 0)
		return "";
	else
		return _currentLine;
}

bool TUScriptParser::ConsumeWhitespace()
{
	if (LineFromCursor().find_first_of(_Whitespace) == std::string::npos)
	{
		return false;
		//MakeError("Expected whitespace.")
	}

	OptConsumeWhitespace();
	return true;
}

void TUScriptParser::OptConsumeWhitespace()
{
	std::string Line = LineFromCursor();
	strtk::remove_leading(_Whitespace, Line);

	AdvanceCursor(LineFromCursor().length() - Line.length());
}

bool TUScriptParser::ConsumeString(const std::string &Str)
{
	if (LineFromCursor().find(Str) == 0)
	{
		AdvanceCursor(Str.length());
	}
	else
	{
		std::string GotStr = (LineFromCursor().length() == 0) ? "<EOL>" : (LineFromCursor().substr(0, Str.length()));
		MakeError(fmt::format("Expected literal '{}', got '{}'.", Str, GotStr));
		return false;
	}
}

bool TUScriptParser::ConsumeUntil(const std::string &Str, std::string &Out)
{
	size_t Offset = LineFromCursor().find(Str);
	if (Offset != std::string::npos)
	{
		Out = LineFromCursor().substr(0u, Offset);
		AdvanceCursor(Offset + 1);
	}
	else
	{
		_cursorPos = _currentLine.length() - 1;
		MakeError(fmt::format("Expected literal '{}'.", Str));
		return false;
	}

	return true;
}

void TUScriptParser::ConsumeRemainder(std::string &Out)
{
	Out = LineFromCursor();
	AdvanceCursor(Out.length());
	//_cursorPos = _currentLine.length() - 1;
}

bool TUScriptParser::ConsumeList(const std::string& Sep, const std::string& End, std::vector<std::string> &Out)
{
	Out.clear();

	bool bFirst = true;
	while (true)
	{
		if (!bFirst)
		{
			OptConsumeWhitespace();
		}

		// EOL handling.
		if (LineFromCursor().length() == 0)
		{
			if (End == "")
			{
				break;
			}
			else
			{
				MakeError(fmt::format("Unexpected EOL (expected literal '{}').", End));
				return false;
			}
		}

		// End-checking
		if (End != "" && LineFromCursor().find(End) == 0u)
		{
			if (!ConsumeString(End))
			{
				MakeError("Catastrophic error consuming list end block.");
				return false;
			}
			break;
		}

		std::string LineContent = "";

		//size_t SepPos = LineFromCursor().find(Sep);
		size_t EndPos = (End != "") ? (LineFromCursor().find(End)) : LineFromCursor().length() - 1;


		// Separator check, get content
		size_t SepPos = LineFromCursor().find(Sep);
 		if (SepPos != std::string::npos && SepPos < EndPos)
		{
			if (!ConsumeUntil(Sep, LineContent))
			{
				return false;
			}

			strtk::remove_leading_trailing(_Whitespace, LineContent);
			if (LineContent.length() == 0)
			{
				MakeError("List item cannot be empty.");
				return false;
			}
			Out.push_back(LineContent);
		}
		else
		{
			// No separator, assume end is next
			if (End != "")
			{
				if (!ConsumeUntil(End, LineContent))
				{
					return false;
				}

				strtk::remove_leading_trailing(_Whitespace, LineContent);
				if (LineContent.length() == 0)
				{
					MakeError("List item cannot be empty.");
					return false;
				}
				Out.push_back(LineContent);

				break;
			}
			else
			{
				ConsumeRemainder(LineContent);

				strtk::remove_leading_trailing(_Whitespace, LineContent);
				if (LineContent.length() == 0)
				{
					MakeError("List item cannot be empty.");
					return false;
				}
				Out.push_back(LineContent);

				break;
			}
		}

		bFirst = false;
	}

	// When it gets here, nothing's gone wrong yet, and all members of the list are parsed.
}

bool TUScriptParser::ConsumeNotes()
{
	OptConsumeWhitespace();

	std::string NoteLine = "";
	ConsumeRemainder(NoteLine);

	return true;
}

bool TUScriptParser::ConsumeNode()
{
	if (!ConsumeString("||"))
		return false;

	OptConsumeWhitespace();

	std::string CommentLine = "";
	if (LineFromCursor().find(">>") == 0u)
	{
		if (!ConsumeString(">>"))
			return false;

		OptConsumeWhitespace();
		ConsumeRemainder(CommentLine);

		_pendingNode._Comment = CommentLine;
	}

	return true;
}

bool TUScriptParser::ConsumeDialogue(/*RawProjectFile_Dialogue &Dest*/)
{
	// Destructive parsing.
	if (!ConsumeString("{"))
	{
		return false;
	}

	std::string Index = "";
	std::vector<std::string> Functions;
	if (LineFromCursor().find('|') < LineFromCursor().find('}'))
	{
		// Functions
		if (!ConsumeWhitespace())
		{
			MakeError("Expected whitespace to the left of '|'.");
			return false;
		}

		if (!ConsumeUntil("|", Index))
		{
			return false;
		}
		else
		{
			strtk::remove_leading_trailing(_Whitespace, Index);
		}

		if (!ConsumeWhitespace())
		{
			MakeError("Expected whitespace to the right of '|'.");
			return false;
		}

		if (!ConsumeList(",", "}", Functions))
		{
			return false;
		}
	}
	else
	{
		if (!ConsumeUntil("}", Index))
		{
			return false;
		}
	}

	OptConsumeWhitespace();	// Don't know if there's going to be a space or not, so just to be sure.

	std::string DialogueText = "";
	ConsumeRemainder(DialogueText);

	RawProjectFile_Dialogue NewDlg(Index, Functions, DialogueText);
	_pendingNode._Dialogues.push_back(NewDlg);
	
	return true;
}

bool TUScriptParser::ConsumeOption(/*RawProjectFile_Dialogue &Dest*/)
{
	if (!ConsumeString("\t"))
	{
		MakeError("Tab expected to precede Option definition.");
		return false;
	}

	// Destructive parsing.
	if (!ConsumeString("["))
	{
		//MakeError("Possible typo:")
		return false;
	}

	std::string Index = "";
	std::vector<std::string> Functions;
	// [(?index) | functions]
	if (LineFromCursor().find('|') < LineFromCursor().find(']'))
	{
		// Functions
		if (!ConsumeWhitespace())
		{
			MakeError("Expected whitespace to the left of '|'.");
			return false;
		}

		if (!ConsumeUntil("|", Index))
		{
			return false;
		}

		if (!ConsumeWhitespace())
		{
			MakeError("Expected whitespace to the right of '|'.");
			return false;
		}

		if (!ConsumeList(",", "]", Functions))
		{
			return false;
		}
	}
	// [index]
	else
	{
		if (!ConsumeUntil("]", Index))
		{
			return false;
		}
	}

	OptConsumeWhitespace();	// Don't know if there's going to be a space or not, so just to be sure.

	RawProjectFile_Option NewOpt("", {}, {}, "");

	std::string OptionText = "";
	if (LineFromCursor().find("//") != std::string::npos)
	{
		std::string FirstVisScript = "";
		if (!ConsumeUntil("//", FirstVisScript))
			return false;

		std::vector<std::string> VisScripts = { FirstVisScript };
		if (!ConsumeList("//", "", VisScripts))
			return false;

		NewOpt._VisibilityScripts = VisScripts;
	}
	else
	{

	}

	std::string DialogueText = "";
	ConsumeRemainder(DialogueText);

	NewOpt._Pointer = Index;
	NewOpt._Text = DialogueText;
	NewOpt._Functions = Functions;

	return true;
}