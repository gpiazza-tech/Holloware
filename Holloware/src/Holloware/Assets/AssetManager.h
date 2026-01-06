#pragma once

namespace Holloware
{
	enum class AssetType : uint8_t;
	class UUID;
	class Asset;

	static class AssetManager
	{
	public:
		static void Init();

		static Asset Get(const std::filesystem::path& path);
		static AssetType PathToAssetType(const std::filesystem::path& path);

		static const std::filesystem::path& GetPath(Asset asset);
		static AssetType GetType(Asset asset);
		static Ref<void> GetData(Asset asset);
	private:
		static Asset Create(const std::filesystem::path& path);
		static void AssetManager::InitData(UUID uuid, AssetType type, const std::filesystem::path& path);
	};
}