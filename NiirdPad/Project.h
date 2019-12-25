#pragma once

#include <filesystem>
#include <map>
#include <string>

#include <rapidjson\schema.h>

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
	bool _bUnsavedChanges = false;
	std::map<std::string, Character*> _Characters;
	rapidjson::SchemaDocument *_ProjectSchema;

	void Unload();	// Resets the Project to its default state.

public:
	/*Project(QNodeView &NodeView, const std::string &Path);
	Project(QNodeView &NodeView, const RawProjectFile &ImportedProject);*/
	Project(QNodeView &NodeView);
	~Project();

	void New();

	const std::string &SavePath() const;
	bool Save();
	bool SaveAs(const std::string &Path = "");

	bool Load(const std::string &Path, std::string *ErrorMessage = nullptr);
	bool Load(const RawProjectFile &ImportedProject, std::string *ErrorMessage = nullptr);

	bool UnsavedChanges() const;
	// Declares there are unsaved changes.
	void DirtyChanges();
	// Declares there are no longer any unsaved changes.
	void CleanChanges();

	Character *NewCharacter(const std::string &Name);
	void RenameCharacter(Character *EditChar, const std::string &Name);
	bool DeleteCharacter(Character *DeleteChar);
	std::map<std::string, Character*> &Characters();
};