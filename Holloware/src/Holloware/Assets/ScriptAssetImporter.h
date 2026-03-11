#pragma once

#include "Holloware/Assets/AssetImporter.h"

namespace Holloware
{
	class ScriptAssetImporter : public AssetImporter
	{
	public:
		bool CanImport(const std::filesystem::path& path) override { return path.extension() == ".c"; }
		nlohmann::json Import(const std::filesystem::path& path);

		Ref<void> Load(const std::filesystem::path& path) override;
	};
}