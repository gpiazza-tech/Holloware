#include <hwpch.h>
#include "TextureAssetImporter.h"

#include "Holloware/Renderer/Texture.h"

namespace Holloware
{
	Ref<void> TextureAssetImporter::Load(const std::filesystem::path& path)
	{
		Ref<Texture2D> texture = Texture2D::Create(path);
		return std::static_pointer_cast<void>(texture);
	}
}