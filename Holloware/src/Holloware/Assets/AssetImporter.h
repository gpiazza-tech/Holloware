#pragma once

#include <filesystem>

namespace Holloware
{
	class AssetImporter
	{
	public:
		virtual bool CanImport(const std::filesystem::path& path) = 0;
		virtual bool NeedsReimport(const std::filesystem::path& path);
		virtual Ref<void> Import(const std::filesystem::path& path) = 0;
	};
}