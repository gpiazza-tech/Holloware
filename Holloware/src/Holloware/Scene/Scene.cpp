#include "hwpch.h"
#include "Scene.h"

#include "Holloware/Scripting/Interpreter.h"
#include "Holloware/Scripting/ScriptData.h"
#include "Holloware/Python/PythonInterpreter.h"
#include "Holloware/Core/Timestep.h"
#include "Holloware/Renderer/Renderer2D.h"
#include "Holloware/Scene/Components.h"
#include "Holloware/Scene/EditorCamera.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Python/PythonEntity.h"
#include "Holloware/Scripting/ScriptInstance.h"

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

			if (sprite.TextureAsset)
			{
				Ref<Texture2D> texture = sprite.TextureAsset.GetData<Texture2D>();
				Ref<SubTexture2D> subTexture = CreateRef<SubTexture2D>(texture, glm::vec2(0, 0), glm::vec2(1, 1));
				Renderer2D::DrawQuad(transform.GetTransform(), subTexture, sprite.Color, (int)entity);
			}
			else
			{
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int)entity);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnStartRuntime()
	{
		// Bind Entity Scripts
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto entity : view)
			{
				auto& sc = view.get<ScriptComponent>(entity);
				std::string source = sc.ScriptAsset.GetData<ScriptData>()->Source;
				Interpreter::BindEntityToScript(source, Entity(entity, this));
			}
		}

		// Call Start
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto entity : view)
			{
				auto& sc = view.get<ScriptComponent>(entity);
				TagComponent& tag = Entity(entity, this).GetComponent<TagComponent>();

				if (sc.Instance)
				{
					sc.Instance->TryCallStart();
				}
			}
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Call Update
		{
			HW_PROFILE_SCOPE("Update Python");

			auto view = m_Registry.view<ScriptComponent>();
			for (auto entity : view)
			{
				auto& sc = view.get<ScriptComponent>(entity);
				TagComponent& tag = Entity(entity, this).GetComponent<TagComponent>();

				if (sc.Instance)
				{
					sc.Instance->TryCallUpdate(ts);
				}
			}
		}

		// Find Camera
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			HW_PROFILE_SCOPE("Find Camera");

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
			HW_PROFILE_SCOPE("Render Sprites");

			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				if (sprite.TextureAsset)
				{
					Ref<Texture2D> texture = sprite.TextureAsset.GetData<Texture2D>();
					Ref<SubTexture2D> subTexture = CreateRef<SubTexture2D>(texture, glm::vec2(0, 0), glm::vec2(1, 1));
					Renderer2D::DrawQuad(transform.GetTransform(), subTexture, sprite.Color, (int)entity);
				}
				else
				{
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, (int)entity);
				}
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnStopRuntime()
	{
		// Call Stop
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto entity : view)
			{
				auto& sc = view.get<ScriptComponent>(entity);
				TagComponent& tag = Entity(entity, this).GetComponent<TagComponent>();

				if (sc.Instance)
				{
					sc.Instance->TryCallStop();
				}
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