#include "hwpch.h"
#include "Entity.h"

namespace Holloware
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

	Entity::Entity(Scene* scene)
		: m_EntityHandle(scene->CreateEntity().m_EntityHandle), m_Scene(scene)
	{
	}
}