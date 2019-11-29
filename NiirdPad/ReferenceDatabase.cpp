#include "ReferenceDatabase.h"

#include "Node.h"

ReferenceDatabase::ReferenceDatabase()
{

}

bool ReferenceDatabase::New(const std::string &Reference, NodeDialogue *Dialogue)
{
	auto Res = Find(Reference);
	// Return false if the reference already exists
	if (Res != nullptr)
		return false;

	_references.insert({ Reference, Dialogue });
	return true;
}

bool ReferenceDatabase::Rename(const std::string &OldReference, const std::string &NewReference)
{
	auto Res = Find(OldReference);
	// Return false if the old reference doesn't exist
	if (Res == nullptr)
		return false;

	_references.erase(OldReference);
	_references.insert({ NewReference, Res });
	return true;
}

bool ReferenceDatabase::Delete(const std::string &Reference)
{
	auto Res = Find(Reference);
	// Return false if the reference doesn't exist
	if (Res == nullptr)
		return false;

	_references.erase(Reference);
	return true;
}

NodeDialogue *ReferenceDatabase::Find(const std::string &Reference)
{
	auto Res = _references.find(Reference);
	if (Res == _references.end())
		return nullptr;
	else
		return Res->second;
}

const std::map<std::string, NodeDialogue*> &ReferenceDatabase::Map() const
{
	return _references;
}