#include "hwpch.h"
#include "Scene.h"

#include "Holloware/Renderer/Renderer2D.h"

#include "Components.h"

#include <glm/glm.hpp>

#include "Entity.h"
#include "Holloware/Python/PythonEntity.h"
#include "Holloware/Python/PythonBinder.h"

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

	void Scene::BindEntityScripts(PythonBinder binder)
	{
		// Bind all python instances to a script
		{
			auto view = m_Registry.view<PythonScriptComponent>();
			for (auto entity : view)
			{
				auto& psc = view.get<PythonScriptComponent>(entity);
				TagComponent& tag = Entity(entity, this).GetComponent<TagComponent>();

				try { binder.BindPythonScriptComponentFunctions(psc, Entity(entity, this)); }
				catch (std::exception e) { HW_CORE_ERROR("{0} Start() error: {1}", tag.Tag, psc.Filepath); }
			}
		}
	}

	void Scene::OnStartRuntime()
	{
		// Python Start
		{
			auto view = m_Registry.view<PythonScriptComponent>();
			for (auto entity : view)
			{
				auto& psc = view.get<PythonScriptComponent>(entity);
				TagComponent& tag = Entity(entity, this).GetComponent<TagComponent>();

				if (psc.Instance)
				{
					try { psc.Instance->OnStart(); }
					catch (std::exception e) { HW_CORE_ERROR("{0} Start() error: {1}", tag.Tag, psc.Filepath); }
				}
				else
				{
					HW_CORE_ERROR("Python instance attached to {0} is null", tag.Tag);
				}
			}
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Python Update
		{
			auto view = m_Registry.view<PythonScriptComponent>();
			for (auto entity : view)
			{
				auto& psc = view.get<PythonScriptComponent>(entity);
				TagComponent& tag = Entity(entity, this).GetComponent<TagComponent>();

				if (psc.Instance)
				{
					try { psc.Instance->OnUpdate(ts); }
					catch (std::exception e) { HW_CORE_ERROR("{0} Update() error: {1}", tag.Tag, psc.Filepath); }
				}
			}
		}

		// Find Camera
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

		// Render sprites
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

				// free heap memory
				delete psc.Instance;
				psc.Instance = nullptr;
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
}