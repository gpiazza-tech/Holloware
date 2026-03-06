#include <hwpch.h>
#include "PythonScriptAssetImporter.h"

#include "Holloware/Python/PythonInterpreter.h"
#include "Holloware/Python/PythonScriptData.h"
#include "Holloware/Serialization/Serializer.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	Ref<void> PythonScriptAssetImporter::Import(const fs::path& path)
	{
		PythonScriptData pythonScriptData = PythonScriptData();

		std::ifstream ifs(path.string());
		std::ostringstream oss;
		oss << ifs.rdbuf();

		pythonScriptData.Code = oss.str();
		pythonScriptData.Attributes = PythonInterpreter::GetAttributes(oss.str());

		return CreateRef<PythonScriptData>(pythonScriptData);
	}
}