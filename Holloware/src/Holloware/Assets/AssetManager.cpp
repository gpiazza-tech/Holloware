#include <hwpch.h>
#include "AssetManager.h"

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Project.h"
#include "Holloware/Core/UUID.h"

#include "Holloware/Assets/Asset.h"
#include "Holloware/Assets/AssetType.h"

#include "Holloware/Renderer/Texture.h"

#include "Holloware/Serialization/Serializer.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	static fs::path s_AssetsPath = "NONE";

	static std::unordered_map<UUID, fs::path> s_PathMap;
	static std::unordered_map<UUID, AssetType> s_TypeMap;
	static std::unordered_map<UUID, Ref<void>> s_DataMap;

	void AssetManager::Init()
	{
		s_AssetsPath = fs::path(Application::Get().GetCurrentProject().GetAssetsPath());

		// Loop over all asset files in project
		for (const auto& entry : fs::recursive_directory_iterator(s_AssetsPath))
		{
			if (!entry.is_directory() && entry.path().extension() != ".meta")
				AssetManager::Create(entry.path());
		}
	}

	Asset AssetManager::Create(const fs::path& path)
	{
		fs::path metadata = fs::path(path.string().append(".meta"));
		UUID uuid;
		if (fs::exists(metadata))
		{
			Serializer serializer = Serializer::LoadFromFile(metadata.string());

			uint64_t intID = 0;
			serializer.Deserialize<uint64_t>(intID, "UUID");
			uuid = intID;
		}
		else
		{
			Serializer serializer = Serializer();
			serializer.Add<uint64_t>(uuid, "UUID");
			serializer.WriteToFile(metadata.string());
		}

		s_PathMap[uuid] = path;
		s_TypeMap[uuid] = PathToAssetType(path);
		InitData(uuid, PathToAssetType(path), path);

		return Asset(uuid);
	}

	const std::filesystem::path& AssetManager::GetPath(Asset asset)
	{
		return s_PathMap[asset];
	}

	AssetType AssetManager::GetType(Asset asset)
	{
		return s_TypeMap[asset];
	}

	Ref<void> AssetManager::GetData(Asset asset)
	{
		return s_DataMap[asset];
	}

	void AssetManager::InitData(UUID uuid, AssetType type, const fs::path& path)
	{
		switch (type)
		{
			case AssetType::TextureAsset:
			{
				Ref<Texture2D> texture = Texture2D::Create(path);
				s_DataMap[uuid] = std::static_pointer_cast<void>(texture);
				break;
			}
			case AssetType::PythonScriptAsset:
			{
				HW_CORE_WARN("AssetType::PythonScriptAsset not implemented", path.string());
				break;
			}
			case AssetType::SceneAsset:
			{
				HW_CORE_WARN("AssetType::SceneAsset not implemented", path.string());
				break;
			}
			default:
			{
				HW_CORE_WARN("Asset type not implemented for file {0}; AssetData.Data is nullptr", path.string());
				s_DataMap[uuid] = nullptr;
				break;
			}
		}
	}

	Asset AssetManager::Get(const fs::path& path)
	{
		Serializer serializer = Serializer::LoadFromFile(path.string().append(".meta"));

		uint64_t intID = 0;
		serializer.Deserialize<uint64_t>(intID, "UUID");

		return Asset(UUID(intID));
	}

	AssetType AssetManager::PathToAssetType(const std::filesystem::path& path)
	{
		if (path.extension() == ".png") return AssetType::TextureAsset;
		else if (path.extension() == ".py") return AssetType::PythonScriptAsset;
		else if (path.extension() == ".hws") return AssetType::SceneAsset;
		
		HW_CORE_WARN("path extension {0} could not be converted to a Holloware::AssetType!", path.extension().string());
		return AssetType::Asset;
	}
}