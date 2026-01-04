#pragma once

#include "Holloware/Core/UUID.h"

namespace Holloware
{
	// namespace std { class string; }
	class AssetManager;

	class Asset
	{
	public:
		// std::string GetPath();
	private:
		Asset(UUID uuid) : m_Handler(uuid) {}
		~Asset() = default;

		UUID m_Handler;

		friend AssetManager;
	};
}