#include "hwpch.h"
#include "SceneHierarchyPanel.h"

#include "Holloware/ImGui/ImGuiUtilities.h"
#include "Holloware/Renderer/Texture.h"

#include "Holloware/Scene/Components.h"
#include "Holloware/Scene/Scene.h"

#include <entt.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>
#include <execution>
#include <filesystem>

namespace Holloware
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		// Sort before drawing
		/*m_Context->m_Registry.sort<entt::entity>([](const entt::entity lhs, const entt::entity rhs) {
			return entt::registry::entity(lhs) < entt::registry::entity(rhs);
			});*/

		auto& view = m_Context->m_Registry.view<entt::entity>();

		// TODO: reverse draw order
		for (auto entityID : m_Context->m_Registry.view<entt::entity>())
		{
			Entity entity = Entity(entityID, m_Context.get());
			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};
		
		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Entity"))
				m_Context->CreateEntity("Entity");
			if (ImGui::MenuItem("Create Abstract Entity"))
				m_Context->CreateAbstractEntity("Abstract Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!m_SelectionContext.HasComponent<TransformComponent>() && ImGui::MenuItem("Transform"))
				{
					m_SelectionContext.AddComponent<TransformComponent>();
					ImGui::CloseCurrentPopup;
				}

				if (!m_SelectionContext.HasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup;
				}

				if (!m_SelectionContext.HasComponent<SpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup;
				}

				if (!m_SelectionContext.HasComponent<PythonScriptComponent>() && ImGui::MenuItem("Python Script"))
				{
					m_SelectionContext.AddComponent<PythonScriptComponent>();
					ImGui::CloseCurrentPopup;
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			DrawEntityNode(entity);
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<IDComponent>())
		{
			auto& id = entity.GetComponent<IDComponent>();
			id.DrawGUI();
		}

		if (entity.HasComponent<TagComponent>())
		{
			auto& tagComponent = entity.GetComponent<TagComponent>();
			tagComponent.DrawGUI();
		}

		DrawComponent<TransformComponent>(entity, "Transform", [](TransformComponent& c)
			{
				c.DrawGUI();
			});

		DrawComponent<CameraComponent>(entity, "Camera", [](CameraComponent& c)
			{
				c.DrawGUI();
			});

		DrawComponent<SpriteRendererComponent>(entity, "Sprite Renderer", [](SpriteRendererComponent& c)
			{
				c.DrawGUI();
			});

		DrawComponent<PythonScriptComponent>(entity, "Python Script", [](PythonScriptComponent& c)
			{
				c.DrawGUI();
			});
	}

	template <typename T>
	void SceneHierarchyPanel::DrawComponent(Entity entity, const char* name, void (*DrawBody)(T&))
	{
		if (entity.HasComponent<T>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap, name);
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2(20, 20)))
			{
				ImGui::OpenPopup(name);
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup(name))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				auto& c = entity.GetComponent<T>();
				DrawBody(c);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}
}