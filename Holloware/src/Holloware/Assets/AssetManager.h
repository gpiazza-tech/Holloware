#pragma once

namespace Holloware
{
	class Asset;

	static class AssetManager
	{
	public:
		static void Init();

		static Asset Get(std::filesystem::path path);
	private:
		static Asset Create(std::filesystem::path path);
	};
}