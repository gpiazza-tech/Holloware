#include <hwpch.h>
#include "SceneAssetImporter.h"

#include "Holloware/Scene/Scene.h"

#include "Holloware/Scene/SceneSerializer.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	Ref<void> SceneAssetImporter::Load(const fs::path& path)
	{
		return SceneSerializer::Deserialize(path);
	}
}