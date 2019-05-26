#pragma once

#include <filesystem>
#include <vector>

#include <QProgressBar>
#include <QThread>

#include "RawProjectFile.h"
#include "TUScriptLexer.h"

class ImportWorker : public QObject
{
	Q_OBJECT

private:
	std::experimental::filesystem::path _TeraurgeDatabasePath;
	std::atomic<bool> _Cancelled;

	std::string _Error;
	std::vector<std::string> _Warnings;
	RawProjectFile _LoadedProject;

	bool LoadCharacters(const std::experimental::filesystem::path &CharactersPath);

public:
	// ENSURE THAT THE PATH EXISTS BEFORE PASSING IT TO IMPORTWORKER!
	ImportWorker(const std::string &TeraurgeDatabasePath);
	virtual ~ImportWorker();

	const std::string &Error() const;
	const std::vector<std::string> &Warnings() const;

public slots:
	void Start();
	void Cancel();

signals:
	void SetTotal(int);
	void SetProgress(int);
	void SetMessage(QString);
	void SendError(std::string);
	void SendData(std::vector<std::string> /*Warnings*/, RawProjectFile);
	void Finished();
};