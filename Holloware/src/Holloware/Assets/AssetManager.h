#pragma once

#include <any>

namespace Holloware
{
	enum class AssetType : uint8_t;
	class UUID;
	class Asset;
	class AssetImporter;
	class AssetUpdateListener;

	static class AssetManager
	{
	public:
		static void Init();

		static Asset Get(const std::filesystem::path& path);

		static const std::filesystem::path& GetPath(Asset asset);
		static Ref<void> GetData(Asset asset);

		static void Reimport(Asset asset);
		static void SetAssetImportedCallback(const std::function<void(Asset)>& func);
	private:
		static Asset Import(const std::filesystem::path& path, std::unique_ptr<AssetImporter>& importer);

		friend AssetUpdateListener;
	};
}