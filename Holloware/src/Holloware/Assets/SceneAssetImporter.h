#pragma once

#include "Holloware/Assets/AssetImporter.h"

namespace Holloware
{
	class SceneAssetImporter : public AssetImporter
	{
	public:
		bool CanImport(const std::filesystem::path& path) override { return path.extension() == ".hws"; }
		Ref<void> Import(const std::filesystem::path& path) override;
	};
}