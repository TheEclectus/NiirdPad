#pragma once

#include <filesystem>
#include <map>
#include <string>

// Character.h
class Character;

// QNodeView.h
class QNodeView;

// RawProjectFile.h
class RawProjectFile;

class Project
{
private:
	QNodeView &_NodeView;
	std::string _savePath = "";	// If empty, assumed to be a new file.
	bool _bUnsavedChanges = true;
	std::map<std::string, Character*> _Characters;

public:
	Project(QNodeView &NodeView, const std::string &Path);
	Project(QNodeView &NodeView, const RawProjectFile &ImportedProject);
	Project(QNodeView &NodeView);

	bool Save();
	bool SaveAs(const std::string &Path);

	Character *NewCharacter(const std::string &Name);
	std::map<std::string, Character*> &Characters();
};