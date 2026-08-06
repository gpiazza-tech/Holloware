#pragma once

#include "AssetType.h"
#include <Perplex/Core/Core.h>
#include <Perplex/Core/UUID.h>
#include <Perplex/Events/Event.h>

#include <memory>
#include <filesystem>
#include <unordered_map>
#include <functional>
#include <vector>

namespace Perplex
{
	class Asset;
	class AssetImporter;
	class AssetUpdateListener;

	class AssetManager
	{
	public:
		AssetManager(const std::filesystem::path& assetsPath);
		~AssetManager();

		void SetEventCallback(const std::function<void(Event&)>& callback) { m_AssetImportedCallback = callback; }

		Asset Get(const std::filesystem::path& path);
		const std::filesystem::path& GetPath(Asset asset);
		Ref<void> GetData(Asset asset);
		Ref<void> LoadData(Asset asset);
		AssetType GetType(Asset asset);
	private:
		void Import(const std::filesystem::path& path);
		Ref<void> Load(Asset asset);
		void Unload(Asset asset);

		friend Asset;
		friend AssetUpdateListener;
	private:
		std::filesystem::path m_AssetsPath;

		std::unordered_map<UUID, std::filesystem::path> m_PathMap{};
		std::unordered_map<UUID, Ref<void>> m_DataMap{};
		std::unordered_map<UUID, AssetType> m_TypeMap{};

		std::vector<std::unique_ptr<AssetImporter>> m_Importers{};

		std::function<void(Event&)> m_AssetImportedCallback{};
	};
}