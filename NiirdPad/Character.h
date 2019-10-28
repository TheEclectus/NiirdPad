#pragma once

#include <map>
#include <string>

// DialogueFile.h
class DialogueFile;

// QNodeView.h
class QNodeView;

// Project.h
class Project;

class Character
{
private:
	Project &_parentProject;
	QNodeView &_nodeView;
	std::string _name;
	std::map<std::string, DialogueFile*> _dialogueFiles;
public:
	Character(Project &ParentProject, QNodeView &NodeView, const std::string &Name);

	const std::string &GetName() const;
	QNodeView &GetNodeView() const;
};