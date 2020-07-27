#pragma once

#include <filesystem>
#include <map>
#include <vector>

#include <QObject>

namespace fs = std::experimental::filesystem;

/// <summary>
/// Creates an asynchronous object that iterates through TU's resource folders.
/// Does no processing, only collates a list of file paths.
/// </summary>
class ResourceDiscriminator : public QObject
{
	Q_OBJECT

public:
	struct Results
	{
		struct Character
		{
			std::string Name	= "";
			std::vector<fs::path> DialogueFiles = {};
			std::vector<fs::path> Sprites	= {};	// Images that show up on the main screen
			std::vector<fs::path> Pictures	= {};	// Charcoal drawing style images
			std::vector<fs::path> MiscFiles	= {};	// Files that aren't used by Niirdpad itself.

			Character() = default;
			Character(const fs::path &Path);
		};

		std::map<std::string, Character> Characters;

		// K = Character identifier, V = path to dialogue files
		//std::multimap<std::string, fs::path> Characters;
	};

	enum class SelectionType : uint8_t
	{
		FullDatabaseFolder,
		IndividualCharacters,
	};

private:
	const std::vector<fs::path> _Targets = {};
	const SelectionType _Type = SelectionType::FullDatabaseFolder;
	
	Results _Results = {};
	std::string _ErrorMessage = "";
	std::vector<std::string> _Warnings;

	bool DatabaseDirectoryIterate();

	/// <summary>
	/// Called by DatabaseDirectoryIterate() when looking through a folder of characters.
	/// </summary>
	bool CharacterDirectoryIterate(fs::path Dir);

public:
	/// <param name="TargetDirectory">Iterates over a full Teraurge/database/ folder.</param>
	//ImportWorker_ResourceDiscriminator(const fs::path &TargetDirectory);
	ResourceDiscriminator(const std::vector<fs::path> &Targets, const SelectionType Type);
	
	const Results &GetResults() const;
	const std::vector<std::string> GetWarnings() const;

public slots:
	void Start();

signals:
	/// <summary>
	///	Results - The Results structure returned; nullptr on failure or cancellation.
	/// std::vector<std::string> - List of warnings/errors.
	/// </summary>
	void SendData(Results, std::vector<std::string>);
	void SendError(std::string);

	void Finished();
};