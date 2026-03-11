#pragma once

#include "Holloware/Assets/AssetImporter.h"

namespace Holloware
{
	class SceneAssetImporter : public AssetImporter
	{
	public:
		bool CanImport(const std::filesystem::path& path) override { return path.extension() == ".hws"; }
		Ref<void> Load(const std::filesystem::path& path) override;
	};
}