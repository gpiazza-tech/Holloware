#pragma once

#include "Holloware/Assets/AssetManager.h"

#include "Holloware/Core/UUID.h"

namespace Holloware
{
	enum class AssetType : uint8_t;

	class Asset
	{
	public:
		~Asset() = default;
		Asset() : m_Handler(false) {};
		Asset(UUID uuid) : m_Handler(uuid) {}
		Asset(std::filesystem::path path);

		const std::filesystem::path& GetPath();
		AssetType GetType();
		Ref<void> GetData();

		template<typename T>
		Ref<T> GetData()
		{
			return std::static_pointer_cast<T>(AssetManager::GetData(m_Handler));
		}
		
		operator UUID() const { return m_Handler; };
		operator uint64_t() const { return (uint64_t)m_Handler; };
		operator bool() const { return m_Handler; };
	private:
		UUID m_Handler;
	};
}