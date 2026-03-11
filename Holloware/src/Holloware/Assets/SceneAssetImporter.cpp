#include <hwpch.h>
#include "SceneAssetImporter.h"

#include "Holloware/Scene/Scene.h"

#include "Holloware/Scene/SceneSerializer.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	Ref<void> SceneAssetImporter::Load(const fs::path& path)
	{
		// TODO: should deserialize .hws into Scene data
		return CreateRef<int>(0); // SceneSerializer::Deserialize(path);
	}
}