#include <hwpch.h>
#include "ScriptAssetImporter.h"

#include "Holloware/Scripting/Interpreter.h"
#include "Holloware/Scripting/ScriptData.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	Ref<void> ScriptAssetImporter::Import(const fs::path& path)
	{
		std::ifstream ifs(path.string());
		std::ostringstream oss;
		oss << ifs.rdbuf();

		ScriptData* scriptData = new ScriptData();
		scriptData->Source = Interpreter::TrimProperties(oss.str());
		scriptData->Properties = Interpreter::FindProperties(oss.str());

		return Ref<ScriptData>(scriptData);
	}
}