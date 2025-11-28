#pragma once

#include "Holloware/Core/Timestep.h"
#include "EditorCamera.h"

#include <glm/detail/type_vec3.hpp>
#include <entt.hpp>

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
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(Timestep ts, const EditorCamera& camera);

		void OnStartRuntime();
		void OnUpdateRuntime(Timestep ts);
		void Scene::OnStopRuntime();

		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}