#include "ImportWorker.h"

namespace fs = std::experimental::filesystem;

ImportWorker::ImportWorker(const std::string &TeraurgeDatabasePath) :
	_TeraurgeDatabasePath(fs::absolute(TeraurgeDatabasePath)),
	_Cancelled(false)
{
	
}

ImportWorker::~ImportWorker()
{

}

void ImportWorker::Start()
{
	// Initial state of loading
	emit SetTotal(0);
	emit SetProgress(0);
	emit SetMessage("Scanning directories...");



	for (auto CurFolder : fs::directory_iterator(_TeraurgeDatabasePath))
	{

	}
}

void ImportWorker::Cancel()
{
	_Cancelled = true;
}