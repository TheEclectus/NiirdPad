#include "DialogueFile.h"

#include "Character.h"
#include "Node.h"
#include "QNodeView.h"
#include "ReferenceDatabase.h"

DialogueFile::DialogueFile(Character &ParentCharacter, const std::string &Filename) :
	_parentCharacter(ParentCharacter),
	_referenceDatabase(new ReferenceDatabase()),
	_filename(Filename)
{

}

Character &DialogueFile::ParentCharacter()
{
	return _parentCharacter;
}

ReferenceDatabase &DialogueFile::GetReferenceDatabase()
{
	return *_referenceDatabase;
}

const std::string &DialogueFile::GetFilename() const
{
	return _filename;
}

// TODO: Implement per-dialogue file camera positions
//QNodeViewCamera &DialogueFile::GetCamera()
//{
//	return _Camera;
//}

const std::vector<Node*> &DialogueFile::GetNodes() const
{
	return _nodes;
}

void DialogueFile::Save(rapidjson::Document &Doc, rapidjson::Value &Value) const
{
	// TODO: Implement DialogueFile comments.
	rapidjson::Value Comment("", 0);
	Value.AddMember("comment", Comment, Doc.GetAllocator());

	rapidjson::Value Filename(_filename.c_str(), _filename.length());
	Value.AddMember("filename", Filename, Doc.GetAllocator());

	rapidjson::Value CameraPos(rapidjson::kObjectType);
	auto &ViewCamera = _parentCharacter.GetNodeView().GetCamera();
	CameraPos.AddMember("x", ViewCamera.ViewBox.x, Doc.GetAllocator());
	CameraPos.AddMember("y", ViewCamera.ViewBox.y, Doc.GetAllocator());
	Value.AddMember("cameraPos", CameraPos, Doc.GetAllocator());

	rapidjson::Value Nodes(rapidjson::kArrayType);
	for (auto &CurNodeIter : _nodes)
	{
		rapidjson::Value CurNode(rapidjson::kObjectType);
		CurNodeIter->Save(Doc, CurNode);

		Nodes.PushBack(CurNode, Doc.GetAllocator());
	}
	Value.AddMember("nodes", Nodes, Doc.GetAllocator());
}

Node *DialogueFile::NewNode(const SDL_Point &Position)
{
	auto &NodeView = _parentCharacter.GetNodeView();

	Node *NewNode = new Node(*this, NodeView, Position);
	_nodes.push_back(NewNode);

	return NewNode;
}