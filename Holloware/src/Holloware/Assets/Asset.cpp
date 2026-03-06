#include <hwpch.h>
#include "Asset.h"

#include "Holloware/Assets/AssetManager.h"

namespace Holloware
{
	Asset::Asset(std::filesystem::path path)
	{
		m_Handler = AssetManager::Get(path);
	}

	const std::filesystem::path& Asset::GetPath()
	{
		return AssetManager::GetPath(*this);
	}

	std::any Asset::GetData()
	{
		return AssetManager::GetData(*this);
	}
}