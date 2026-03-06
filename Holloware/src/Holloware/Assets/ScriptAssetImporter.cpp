#include <hwpch.h>
#include "ScriptAssetImporter.h"

#include "Holloware/Scripting/Interpreter.h"
#include <Holloware/Scripting/ScriptData.h>

namespace Holloware
{
	namespace fs = std::filesystem;

	Ref<void> ScriptAssetImporter::Import(const fs::path& path)
	{
		ScriptData scriptData = ScriptData();

		std::ifstream ifs(path.string());
		std::ostringstream oss;
		oss << ifs.rdbuf();

		scriptData.Source = oss.str();
		scriptData.Properties = Interpreter::GetProperties(oss.str());

		return CreateRef<ScriptData>(scriptData);
	}
}