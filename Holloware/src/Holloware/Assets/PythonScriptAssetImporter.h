#pragma once

#include "Holloware/Assets/AssetImporter.h"

namespace Holloware
{
	class PythonScriptAssetImporter : public AssetImporter
	{
	public:
		bool CanImport(const std::filesystem::path& path) override { return path.extension() == ".py"; }
		Ref<void> Import(const std::filesystem::path& path) override;
	};
}