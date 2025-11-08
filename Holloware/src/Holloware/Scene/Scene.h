#pragma once

#include <entt.hpp>

#include "Holloware/Core/Timestep.h"

namespace Holloware
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateAbstractEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}