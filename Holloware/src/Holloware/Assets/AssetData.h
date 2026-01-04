#pragma once

namespace Holloware
{
	namespace fs = std::filesystem;

	struct AssetData
	{
		AssetData(fs::path path) : Path(path) {}

		fs::path Path;
	};
}