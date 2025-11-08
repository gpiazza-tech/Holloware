#include "hwpch.h"
#include "Scene.h"

#include "Holloware/Renderer/Renderer2D.h"

#include "Components.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Holloware
{
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	Entity Scene::CreateAbstractEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto& entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}