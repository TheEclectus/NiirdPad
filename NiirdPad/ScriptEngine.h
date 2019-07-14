#pragma once

/// <summary>
/// What it is:
/// - A class to verify individual script strings with a function call and return success information
/// What it is not:
/// - Owning of TUScript scripts
/// </summary>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <unordered_map>
#include <vector>

class ScriptEngine
{
private:
	sol::state _state;
	std::unordered_map<std::string, sol::function> _validationFunctions;
public:
	ScriptEngine();

	/// <returns>
	/// True on success, false on error; ConnectionKeys will be filled with any connection options
	/// provided by the function 
	/// </returns>
	/// <param name="Script">The full line of script being validated.</param>
	bool bValidateScript(const std::string &Script, std::vector<std::string> &ConnectionKeys, std::string &ErrorString);
};