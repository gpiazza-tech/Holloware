#include "hwpch.h"
#include "Scene.h"

#include "Holloware/Renderer/Renderer2D.h"

#include "Components.h"

#include <glm/glm.hpp>

#include "Entity.h"
#include "ScriptableEntity.h"
#include "Holloware/Python/PythonEntity.h"

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
		Entity entity = Entity(m_Registry.create(), this);

		entity.AddComponent<IDComponent>();
		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	Entity Scene::CreateAbstractEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);

		entity.AddComponent<IDComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts, const EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			if (sprite.SubTexture)
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.SubTexture, sprite.Color, (int)entity);
			else
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnStartRuntime(PythonBinder pyBinder)
	{
		// executing and binding python scripts
		{
			// binding and executing
			auto view = m_Registry.view<PythonScriptComponent>();
			for (auto entity : view)
			{
				auto& psc = view.get<PythonScriptComponent>(entity);
				pyBinder.BindPythonScriptComponentFunctions(psc);
			}

			// calling start
			for (auto entity : view)
			{
				auto& psc = view.get<PythonScriptComponent>(entity);
				psc.Instance->OnStart();
			}
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Python Scripting
		{
			auto view = m_Registry.view<PythonScriptComponent>();
			for (auto entity : view)
			{
				auto& psc = view.get<PythonScriptComponent>(entity);
				psc.Instance->OnUpdate(ts);
			}
		}

		// Render sprites
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.GetTransform();
					break;
				}
			}
		}

		// Update Camera
		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				if (sprite.SubTexture)
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.SubTexture, sprite.Color, (int)entity);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int)entity);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnStopRuntime()
	{
		{
			// releasing python instances
			auto view = m_Registry.view<PythonScriptComponent>();
			for (auto entity : view)
			{
				auto& psc = view.get<PythonScriptComponent>(entity);
				delete psc.Instance;
			}
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<PythonScriptComponent>(Entity entity, PythonScriptComponent& component)
	{

	}
}