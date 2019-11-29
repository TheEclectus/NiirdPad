#pragma once

#include <map>
#include <string>

// Node.h
class NodeDialogue;

class ReferenceDatabase
{
private:
	std::map<std::string, NodeDialogue*> _references;

public:
	ReferenceDatabase();
	ReferenceDatabase(const ReferenceDatabase&) = delete;	// Disable copy operator
	ReferenceDatabase(ReferenceDatabase&&) = delete;		// Disable move operator

	bool New(const std::string &Reference, NodeDialogue *Dialogue);
	bool Rename(const std::string &OldReference, const std::string &NewReference);
	bool Delete(const std::string &Reference);

	NodeDialogue *Find(const std::string &Reference);
	const std::map<std::string, NodeDialogue*> &Map() const;
};