#include <hwpch.h>
#include "AssetImporter.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	bool AssetImporter::NeedsReimport(const fs::path& path)
	{
		fs::path meta = path.string() + ".meta";

		if (!fs::exists(meta))
			return true;

		if (fs::last_write_time(path) > fs::last_write_time(meta))
			return true;

		return false;
	}

	nlohmann::json AssetImporter::Import(const fs::path& path)
	{
		return nlohmann::json();
	}
}