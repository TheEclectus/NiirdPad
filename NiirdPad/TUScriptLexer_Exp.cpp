#include "TUScriptLexer_Exp.h"

#include <vector>

#include "format.h"

//std::string TestDiag = R"(NOTES
//This is where some random-ass notes gonna go
//					wheeeeeeeeeeee I'm over here
//   now I'm back over here whatcha gonna do
//NOTES
//
//||
//{start | hide_character, counter falmilen_anger + 0} You are standing at the entrance of a cave. If you listen closely, whistling echoing sounds can occasionally be heard from the inside.
//	[test_option] And here's an option.
//||)";
std::string TestDiag = "	[blap] You are standing at the entrance, of a cave. If you listen closely, whistling echoing sounds can occasionally, be heard from the inside.";

TUScriptParser::TUScriptParser()
{
	RawProjectFile_ScriptFile()
}

void TUScriptParser::MakeError(const std::string &Message)
{
	if (_errorMessage.length() > 0)
		_errorMessage += "\n";
	_errorMessage = fmt::format("{}:{}: {}", _lineNum, _cursorPos, Message);
}

void TUScriptParser::Parse()
{
	// Split the file into lines.
	std::vector<std::string> Parts = {};
	strtk::parse(TestDiag, "\n", Parts);

	//strtk::remove_empty_strings(Parts);

	_parserState.push(ParserState::Base);

	while (_lineNum <= Parts.size() - 1)
	{
		_currentLine = Parts[_lineNum];
		_cursorPos = 0u;

		// Valid branch states should be Notes or Node
		if (_parserState.top() == ParserState::Base)
		{
			if (_currentLine.find("NOTES") == 0u)
			{
				ConsumeNotes();
			}

			if (_currentLine.find("||") == 0u)
			{

			}

			if (_currentLine.find('{') == 0u)
				ConsumeDialogue();

			if (_currentLine.find('\t') == 0u)
				ConsumeOption();
		}


		if (_currentLine.length() > 0)
		{
			MakeError("Unconsumed characters before end-of-line.");
			return;
		}
		_lineNum++;
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
		std::string GotStr = LineFromCursor().substr(Str.length());
		MakeError(fmt::format("Expected literal '{}', got {}.", Str, GotStr));
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

	std::string OptionText = "";
	if (LineFromCursor().find("//") != std::string::npos)
	{
		std::string FirstVisScript = "";
		if (!ConsumeUntil("//", FirstVisScript))
			return false;

		std::vector<std::string> Functions = { FirstVisScript };
		if (!ConsumeList("//", "", Functions))
			return false;
	}
	else
	{

	}

	std::string DialogueText = "";
	ConsumeRemainder(DialogueText);

	return true;
}