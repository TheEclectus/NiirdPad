#pragma once

#include <map>
#include <string>

#include <rapidjson\document.h>
#include <rapidjson\rapidjson.h>

// DialogueFile.h
class DialogueFile;

// QNodeView.h
class QNodeView;

// Project.h
class Project;

class Character
{
	friend class Project;
private:
	Project &_parentProject;
	QNodeView &_nodeView;	// Used in Node constructor for DialogueFiles.
	std::string _name;
	std::map<std::string, DialogueFile*> _dialogueFiles;
public:
	Character(Project &ParentProject, QNodeView &NodeView, const std::string &Name);

	const std::string &GetName() const;
	QNodeView &GetNodeView() const;

	DialogueFile *NewDialogueFile(const std::string &Filename);
	void RenameDialogueFile(DialogueFile *Dlg, const std::string &Name);
	std::map<std::string, DialogueFile*> &DialogueFiles();

	void Save(rapidjson::Document &Doc, rapidjson::Value &Value) const;
};