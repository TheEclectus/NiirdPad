#include "ScriptEngine.h"

#include <filesystem>

#include <format.h>
#include <QMessageBox>
#include <strtk.hpp>

ScriptEngine::ScriptEngine()
{
	// TODO: Implement the config engine. For now it'll be hardcoded.
	_path = "./TeraurgeV2.lua";

	// Initialize the Lua state.
	_state.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

	ReloadFromFile();
}

bool ScriptEngine::ReloadFromFile()
{
	if (_path.length() == 0 || !std::experimental::filesystem::exists(_path))
		return false;

	auto LoadResult = _state.do_file(_path, sol::load_mode::text);
	if (!LoadResult.valid())
	{
		sol::error Err = LoadResult;
		std::string ErrMsg = Err.what();

		QMessageBox MsgBox;
		MsgBox.setWindowTitle("Lua Error");
		MsgBox.setText(ErrMsg.c_str());
		MsgBox.setIcon(QMessageBox::Icon::Warning);
		MsgBox.exec();

		return false;
	}

	if (LoadResult.get_type() != sol::type::table)
	{
		QMessageBox MsgBox;
		MsgBox.setWindowTitle("Error");
		MsgBox.setText("Type returned by file is not a Lua table.");
		MsgBox.setIcon(QMessageBox::Icon::Warning);
		MsgBox.exec();

		return false;
	}

	sol::table ResultTable = LoadResult;

	sol::object TUScriptVersion = ResultTable["Version"];
	if (TUScriptVersion == sol::nil)
	{
		QMessageBox MsgBox;
		MsgBox.setWindowTitle("Error");
		MsgBox.setText("Table returned by file does not contain 'Version' string.");
		MsgBox.setIcon(QMessageBox::Icon::Warning);
		MsgBox.exec();

		return false;
	}
	else
	{
		std::string TUScriptVerStr = TUScriptVersion.as<std::string>();
		size_t DotPos = TUScriptVerStr.find('.');
		if (DotPos == std::string::npos)
		{
			QMessageBox MsgBox;
			MsgBox.setWindowTitle("Error");
			MsgBox.setText("'Version' string is ill-formed.");
			MsgBox.setIcon(QMessageBox::Icon::Warning);
			MsgBox.exec();

			return false;
		}

		std::string MinusDotStr = TUScriptVerStr.substr(0, DotPos) + TUScriptVerStr.substr(DotPos + 1);
		if (MinusDotStr.find_first_not_of("1234567890") != MinusDotStr.npos)
		{
			QMessageBox MsgBox;
			MsgBox.setWindowTitle("Error");
			MsgBox.setText("'Version' string is ill-formed.");
			MsgBox.setIcon(QMessageBox::Icon::Warning);
			MsgBox.exec();

			return false;
		}

		_TUScriptVersion = TUScriptVerStr;
	}

	sol::function ExtractKeysFromFunctionLine = ResultTable["ExtractKeyPairs"];
	if (ExtractKeysFromFunctionLine == sol::nil)
	{
		QMessageBox MsgBox;
		MsgBox.setWindowTitle("Error");
		MsgBox.setText("Table returned by file does not contain 'ExtractKeyPairs' function.");
		MsgBox.setIcon(QMessageBox::Icon::Warning);
		MsgBox.exec();

		return false;
	}
	_extractKeysFromFunctionLine = ExtractKeysFromFunctionLine;

	sol::table DialogueTable = ResultTable["Dialogue"];
	if (DialogueTable == sol::nil)
	{
		QMessageBox MsgBox;
		MsgBox.setWindowTitle("Error");
		MsgBox.setText("Table returned by file does not contain 'Dialogue' table.");
		MsgBox.setIcon(QMessageBox::Icon::Warning);
		MsgBox.exec();

		return false;
	}

	sol::table VisibilityTable = ResultTable["Visibility"];
	if (DialogueTable == sol::nil)
	{
		QMessageBox MsgBox;
		MsgBox.setWindowTitle("Error");
		MsgBox.setText("Table returned by file does not contain 'Visibility' table.");
		MsgBox.setIcon(QMessageBox::Icon::Warning);
		MsgBox.exec();

		return false;
	}

	_validationFunctions.clear();
	for (auto i : DialogueTable)
	{
		std::string FunctionName = i.first.as<std::string>();
		sol::function DialogueFunction = i.second.as<sol::function>();

		_validationFunctions.insert({ FunctionName, DialogueFunction });
	}

	_visibilityConditions.clear();
	for (auto i : VisibilityTable)
	{
		std::string ConditionName = i.first.as<std::string>();
		sol::function VisibilityFunction = i.second.as<sol::function>();

		_visibilityConditions.insert({ ConditionName, VisibilityFunction });
	}

	return true;
}

bool ScriptEngine::LoadFromFile(std::string &Path)
{
	if (Path.length() == 0 || !std::experimental::filesystem::exists(Path))
		return false;

	_path = Path;

	return ReloadFromFile();
}

bool ScriptEngine::bScriptIsValid(const std::string &Script, std::vector<std::string> &ConnectionKeys, std::string &ErrorString) const
{
	ConnectionKeys.clear();

	std::vector<std::string> Parts = {};
	strtk::parse(Script, " ", Parts);
	
	std::string FunctionName = Parts[0];
	Parts.erase(Parts.begin());

	// TODO: On saving, parse out everything valid but improper (basically just extra whitespace)
	strtk::remove_empty_strings(Parts);

	auto FindRes = _validationFunctions.find(FunctionName);
	if (FindRes == _validationFunctions.end())
	{
		ErrorString = fmt::format("Function '{0}' not defined.", FunctionName);
		//ConnectionKeys.push_back("__default__");
		return false;
	}

	//std::vector<std::string> Keys = {};
	//std::string Error = "";

	auto FunctionRes = (*FindRes).second(Parts);
	if (!FunctionRes.valid())
	{
		ErrorString = FunctionRes;
		//ConnectionKeys.push_back("__default__");
		return false;
	}

	bool bIsValid = false;
	std::tuple<bool, sol::object, std::string> Res = FunctionRes;
	bIsValid = std::get<0>(Res);
	sol::object KeysObject = std::get<1>(Res);
	ErrorString = std::get<2>(Res);

	if (KeysObject != sol::nil)
	{
		sol::table KeysTable = KeysObject;
		for (auto CurKey : KeysTable)
		{
			ConnectionKeys.push_back(CurKey.second.as<std::string>());
		}
	}
	else
	{
		//ConnectionKeys.push_back("__default__");
	}

	return bIsValid;
}

bool ScriptEngine::bVisConditionIsValid(const std::string &Script, std::string &ErrorString) const
{
	if (Script.substr(0, 2) != "//")
	{
		ErrorString = "Visibility condition must begin with '//'.";
		return false;
	}

	std::string ScriptLine = Script.substr(2);

	std::vector<std::string> Parts = {};
	strtk::parse(ScriptLine, ".", Parts);

	if (Parts.size() == 0)
	{
		ErrorString = "'showif' or 'hideif' expected.";
		return false;
	}

	if (Parts[0] != "showif" && Parts[0] != "hideif")
	{
		ErrorString = "Condition must be lead with 'showif' or 'hideif'.";
		return false;
	}
	Parts.erase(Parts.begin());

	if (Parts.size() < 1)
	{
		ErrorString = "Visibility condition must be specified.";
		return false;
	}

	std::string FunctionName = Parts[0];
	Parts.erase(Parts.begin());

	auto FindRes = _visibilityConditions.find(FunctionName);
	if (FindRes == _visibilityConditions.end())
	{
		ErrorString = fmt::format("Function '{0}' not defined.", FunctionName);
		return false;
	}

	//std::vector<std::string> Keys = {};
	//std::string Error = "";

	auto FunctionRes = (*FindRes).second(Parts);
	if (!FunctionRes.valid())
	{
		ErrorString = FunctionRes;
		return false;
	}

	bool bIsValid = false;
	std::tuple<bool, std::string> Res = FunctionRes;
	bIsValid = std::get<0>(Res);
	ErrorString = std::get<1>(Res);

	return bIsValid;
}

bool ScriptEngine::ExtractKeyPairs(const std::string &ScriptLine, std::unordered_map<std::string, std::string> &KeyPairs, std::string &ReassembledLine, std::string &ErrorString) const
{
	KeyPairs.clear();
	ReassembledLine = "";

	std::vector<std::string> Parts = {};
	strtk::parse(ScriptLine, " ", Parts);

	std::string FunctionName = Parts[0];
	Parts.erase(Parts.begin());

	// TODO: On saving, parse out everything valid but improper (basically just extra whitespace)
	strtk::remove_empty_strings(Parts);

	auto Res = _extractKeysFromFunctionLine(FunctionName, Parts);
	if (!Res.valid())
	{
		sol::error Err = Res;
		std::string ErrMsg = Err.what();
	}

	//std::tuple<sol::object, std::string> Res = _extractKeysFromFunctionLine(FunctionName, Parts);
	//sol::table KeyPairsTable = std::get<0>(Res);
	//std::string FuncErrorString = std::get<1>(Res);

	sol::object KeyPairsObj = Res[0];
	//sol::table KeyPairsTable = Res[0];
	std::string FuncErrorString = Res[1];
	std::string ReassembledString = Res[2];

	bool bSuccessful = KeyPairsObj != sol::nil;
	if (!bSuccessful)
	{
		ErrorString = FuncErrorString;
		return false;
	}

	sol::table KeyPairsTable = KeyPairsObj;

	// Key, Index pairs
	if (KeyPairsTable.size() == 0)
	{
		ReassembledLine = ScriptLine;
		return true;
	}

	ReassembledLine = ReassembledString;
	for (auto CurIndex : KeyPairsTable)
	{
		sol::table CurTable = CurIndex.second;
		
		KeyPairs.insert({ CurTable[1].get<std::string>(), CurTable[2].get<std::string>() });
	}
	return true;
}

const std::string &ScriptEngine::VersionString() const
{
	return _TUScriptVersion;
}