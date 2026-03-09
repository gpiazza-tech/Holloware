#include <hwpch.h>
#include "AssetManager.h"

#include "Holloware/Assets/Asset.h"
#include "Holloware/Assets/AssetUpdateListener.h" 
#include "Holloware/Assets/AssetImporter.h"
#include "Holloware/Assets/TextureAssetImporter.h"
#include "Holloware/Assets/ScriptAssetImporter.h"

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Project.h"
#include "Holloware/Core/UUID.h"

#include "Holloware/Serialization/Serializer.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	static fs::path s_AssetsPath = "NONE";

	static std::unordered_map<UUID, fs::path> s_PathMap;
	static std::unordered_map<UUID, Ref<void>> s_DataMap;

	static std::vector<std::unique_ptr<AssetImporter>> s_Importers;

	static std::function<void(Asset)> s_OnAssetImported;

	void AssetManager::Init()
	{
		s_AssetsPath = fs::path(Application::Get().GetCurrentProject().GetAssetsPath());

		// Register importers
		s_Importers.push_back(std::make_unique<TextureAssetImporter>());
		s_Importers.push_back(std::make_unique<ScriptAssetImporter>());

		// Loop over all asset files in project
		for (const auto& entry : fs::recursive_directory_iterator(s_AssetsPath))
		{
			if (!entry.is_directory() && entry.path().extension() != ".meta")
			{
				for (auto& importer : s_Importers)
				{
					if (importer->CanImport(entry.path()))
						Import(entry.path(), importer);
				}
			}
		}

		// Start Watcher
		efsw::FileWatcher* fileWatcher = new efsw::FileWatcher();
		AssetUpdateListener* listener = new AssetUpdateListener();
		efsw::WatchID watchID = fileWatcher->addWatch(s_AssetsPath.string(), listener, true);
		fileWatcher->watch();
	}

	void AssetManager::SetAssetImportedCallback(const std::function<void(Asset)>& func)
	{
		s_OnAssetImported = func;
	}

	void AssetManager::Reimport(Asset asset)
	{
		const fs::path& path = GetPath(asset);

		for (auto& importer : s_Importers)
		{
			if (importer->CanImport(path))
			{ 
				s_DataMap[asset] = importer->Import(path);
				s_OnAssetImported(asset);
			}
		}
	}

	Asset AssetManager::Import(const std::filesystem::path& path, std::unique_ptr<AssetImporter>& importer)
	{
		fs::path meta = fs::path(path.string().append(".meta"));
		UUID uuid;
		if (fs::exists(meta))
		{
			Serializer serializer = Serializer::LoadFromFile(meta.string());

			uint64_t intID = 0;
			serializer.Deserialize<uint64_t>(intID, "UUID");
			uuid = intID;
		}
		else
		{
			Serializer serializer = Serializer();
			serializer.Add<uint64_t>(uuid, "UUID");
			serializer.WriteToFile(meta.string());
		}

		s_PathMap[uuid] = path;
		s_DataMap[uuid] = importer->Import(path);

		return Asset(uuid); 
	}

	const std::filesystem::path& AssetManager::GetPath(Asset asset)
	{
		if (s_PathMap.find(asset) != s_PathMap.end())
		{
			return s_PathMap[asset];
		}
		return "";
	}

	Ref<void> AssetManager::GetData(Asset asset)
	{
		if (s_DataMap.find(asset) != s_DataMap.end())
		{
			return s_DataMap[asset];
		}
		return nullptr;
	}

	Asset AssetManager::Get(const fs::path& path)
	{
		if (!fs::exists(path.string().append(".meta"))) return Asset();

		Serializer serializer = Serializer::LoadFromFile(path.string().append(".meta"));

		uint64_t intID = 0;
		serializer.Deserialize<uint64_t>(intID, "UUID");

		return Asset(UUID(intID));
	}
}