#pragma once

#include "Holloware/Assets/AssetImporter.h"

namespace Holloware
{
	class TextureAssetImporter : public AssetImporter
	{
	public:
		TextureAssetImporter() = default;
		~TextureAssetImporter() {};

		bool CanImport(const std::filesystem::path& path) override { return path.extension().string() == ".png"; };
		Ref<void> Import(const std::filesystem::path& path) override;
	};
}