#include <Perplex/pch.h>
#include <Perplex/Assets/Asset.h>

#include <Perplex/Core/Application.h>
#include <Perplex/Assets/AssetManager.h>
#include <Perplex/Assets/AssetType.h>

#include <nlohmann/json_fwd.hpp>
#include <nlohmann/json.hpp>

#include <filesystem>

namespace Perplex
{
	Ref<void> GetAssetData(UUID handler)
	{
		return Application::Get().GetAssetManager().GetData(handler);
	}

	Ref<void> LoadAssetData(UUID handler)
	{
		return Application::Get().GetAssetManager().LoadData(handler);
	}

	Asset::Asset(const std::filesystem::path& path)
	{
		m_Handler = Application::Get().GetAssetManager().Get(path);
	}

	const std::filesystem::path& Asset::GetPath() const
	{
		return Application::Get().GetAssetManager().GetPath(*this);
	}

	std::filesystem::path Asset::GetName() const
	{
		return Application::Get().GetAssetManager().GetPath(*this).filename();
	}

	AssetType Asset::GetType() const
	{
		return Application::Get().GetAssetManager().GetType(*this);
	}

	void to_json(nlohmann::json& json, const Asset& asset) { json = (uint64_t)asset.m_Handler; }
	void from_json(const nlohmann::json& json, Asset& asset)
	{
		asset.m_Handler = json.get<uint64_t>();
		Application::Get().GetAssetManager().GetData(asset);
	}
}