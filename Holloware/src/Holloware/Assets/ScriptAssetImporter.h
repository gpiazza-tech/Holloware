#pragma once

#include "Holloware/Assets/AssetImporter.h"

namespace Holloware
{
	class ScriptAssetImporter : public AssetImporter
	{
	public:
		bool CanImport(const std::filesystem::path& path) override { return path.extension() == ".c"; }
		Ref<void> Import(const std::filesystem::path& path) override;
	};
}