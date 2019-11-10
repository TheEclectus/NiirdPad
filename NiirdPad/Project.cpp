#include "Project.h"

#include "Character.h"
#include "DialogueFile.h"
#include "Node.h"
#include "RawProjectFile.h"

Project::Project(QNodeView &NodeView, const std::string &Path) :
	Project(NodeView)
{


	_bUnsavedChanges = false;
}

Project::Project(QNodeView &NodeView, const RawProjectFile &ImportedProject) :
	Project(NodeView)
{
	// TODO: Replace CurrentOffset with a proper layout algorithm.

	int CurrentOffset = 0;
	for (auto &CurrentCharacter : ImportedProject.Characters())
	{
		const std::string &CharacterName = CurrentCharacter.first;
		const auto &CharacterData = CurrentCharacter.second;

		Character *NewChar = NewCharacter(CharacterName);
		for (auto &CurrentFile : CharacterData.Files())
		{
			const std::string &FileName = CurrentFile.first;
			auto &FileData = CurrentFile.second;

			DialogueFile *NewFile = NewChar->NewDialogueFile(FileName);
			for (auto &CurrentNode : FileData.Nodes())
			{
				Node *NewNode = NewFile->NewNode();
				// HACK: Right here, see TODO above.
				NewNode->SetPosition({ CurrentOffset, 0 });

				const std::string &Comment = CurrentNode.Comment();
				NewNode->SetComment(Comment);

				for (auto &CurrentDialogue : CurrentNode.Dialogues())
				{
					auto NewDlg = NewNode->AddDialogue(CurrentDialogue.Reference());
					NewDlg->SetAll(CurrentDialogue.Scripts(), CurrentDialogue.Dialogue());
				}

				for (auto &CurrentOption : CurrentNode.Options())
				{
					auto NewOpt = NewNode->AddOption();
					NewOpt->SetAll(CurrentOption.VisibilityScripts(), CurrentOption.Functions(), CurrentOption.Text());
				}
			}
		}
	}
}

Project::Project(QNodeView &NodeView) :
	_NodeView(NodeView)
{

}

Character *Project::NewCharacter(const std::string &Name)
{
	Character *NewCharacter = new Character(*this, _NodeView);
	_Characters.insert({ Name, NewCharacter });

	return NewCharacter;
}

std::map<std::string, Character*> &Project::Characters()
{
	return _Characters;
}