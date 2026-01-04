#include <hwpch.h>
#include "AssetManager.h"

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Project.h"
#include "Holloware/Core/UUID.h"

#include "Holloware/Assets/Asset.h"
#include "Holloware/Assets/AssetData.h"

#include "Holloware/Serialization/Serializer.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	static fs::path s_AssetsPath = "NONE";
	static std::unordered_map<UUID, AssetData*> s_DataMap;

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

	Asset AssetManager::Create(fs::path path)
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
			uuid = UUID();

			Serializer serializer = Serializer();
			serializer.Add<uint64_t>(uuid, "UUID");
			serializer.WriteToFile(metadata.string());
		}
		s_DataMap[uuid] = new AssetData(path);

		return Asset(uuid);
	}

	Asset AssetManager::Get(fs::path path)
	{
		Serializer serializer = Serializer::LoadFromFile(path.string().append(".meta"));

		uint64_t intID = 0;
		serializer.Deserialize<uint64_t>(intID, "UUID");

		return Asset(UUID(intID));
	}
}