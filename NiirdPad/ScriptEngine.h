#pragma once

/// <summary>
/// What it is:
/// - A class to verify individual script strings with a function call and return success information
/// What it is not:
/// - Owning of TUScript scripts
/// </summary>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <map>
#include <vector>

class ScriptEngine
{
private:
	std::string _path;
	sol::state _state;
	std::string _TUScriptVersion;
	sol::safe_function _extractKeysFromFunctionLine;
	std::map<std::string, sol::safe_function> _validationFunctions;
	std::map<std::string, sol::safe_function> _visibilityConditions;

public:
	ScriptEngine();

	/// <summary>
	/// Reloads functions from current file.
	/// </summary>
	/// <returns></returns>
	bool ReloadFromFile();

	/// <summary>
	/// Loads functions from a new Lua file, and points the ScriptEngine to reload from that file.
	/// </summary>
	/// <returns></returns>
	bool LoadFromFile(std::string &Path);

	/// <returns>
	/// True on success, false on error; ConnectionKeys will be filled with any connection options
	/// provided by the function. Remember, ConnectionKeys.size() == 0 means it's non-index-modifying.
	/// </returns>
	/// <param name="Script">The full line of script being validated.</param>
	bool bScriptIsValid(const std::string &Script, std::vector<std::string> &ConnectionKeys, std::string &ErrorString) const;

	/// <returns>
	/// True on success, false on error.
	/// </returns>
	/// <param name="Script">The full line of script being validated.</param>
	bool bVisConditionIsValid(const std::string &Script, std::string &ErrorString) const;

	// Returns true if ScriptLine does not contain an IM function
	// Returns true if ScriptLine contains an IM function and is successfully extracted
	// Returns false if ScriptLine contains an IM function and fails
	bool ExtractKeyPairs(const std::string &ScriptLine, std::unordered_map<std::string, std::string> &KeyPairs, std::string &ReassembledLine, std::string &ErrorString) const;

	const std::string &VersionString() const;
};