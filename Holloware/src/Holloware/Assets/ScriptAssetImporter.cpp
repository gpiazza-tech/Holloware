#include <hwpch.h>
#include "ScriptAssetImporter.h"

#include "Holloware/Scripting/Interpreter.h"
#include "Holloware/Scripting/ScriptData.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	nlohmann::json ScriptAssetImporter::Import(const std::filesystem::path& path)
	{
		nlohmann::json data = nlohmann::json();

		std::ifstream ifs(path.string());
		std::ostringstream oss;
		oss << ifs.rdbuf();

		data["source"] = Interpreter::TrimProperties(oss.str());
		data["properties"] = Interpreter::FindProperties(oss.str());

		return data;
	}

	Ref<void> ScriptAssetImporter::Load(const fs::path& path)
	{
		nlohmann::json data = JsonHelper::LoadFromFile(path.string().append(".meta"))["data"];

		ScriptData scriptData = ScriptData();
		scriptData.Source = data["source"].get<std::string>();
		scriptData.Properties = data["properties"].get<std::vector<ScriptProperty>>();

		return CreateRef<ScriptData>(scriptData);
	}
}