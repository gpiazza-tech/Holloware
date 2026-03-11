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

	void to_json(nlohmann::json& json, const Asset& asset) { json = (uint64_t)asset.m_Handler; }
	void from_json(const nlohmann::json& json, Asset& asset) { asset.m_Handler = json.get<uint64_t>(); }
}