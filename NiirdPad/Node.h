#pragma once

#include <vector>

#include <SDL.h>

// GraphicsBlocks.h
class GraphicsBlock_NodeInputBox;
class GraphicsBlock_NodeOutputBox;

class NodeInputBox
{
private:
	GraphicsBlock_NodeInputBox *_graphics;

	std::string _reference, _script, _dialogue;
public:
	NodeInputBox(const std::string &Reference, const std::string &Script = "", const std::string &Dialogue = "");

	void SetAll(const std::string &Reference, std::string &Script, const std::string &Dialogue);
	void SetReference(const std::string &Reference);
	void SetScriptLines(const std::string &Script);
	void SetDialogue(const std::string &Dialogue);
};