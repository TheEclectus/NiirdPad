#include "ScriptEngine.h"

#include <filesystem>

#include <QMessageBox>

ScriptEngine::ScriptEngine()
{
	// TODO: Implement the config engine. For now it'll be hardcoded.
	_path = "./TeraurgeV2.lua";

	// Initialize the Lua state.
	_state.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
}

bool ScriptEngine::ReloadFromFile()
{
	if (_path.length() == 0 || !std::experimental::filesystem::exists(_path))
		return false;
}

bool ScriptEngine::LoadFromFile(std::string &Path)
{
	if (Path.length() == 0 || !std::experimental::filesystem::exists(Path))
		return false;

	_path = Path;

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

	
	for (auto i : DialogueTable)
	{
		std::string FunctionName = i.first.as<std::string>();
	}
}