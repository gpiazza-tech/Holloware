#include "hwpch.h"
#include "Entity.h"

#include "Holloware/Serialization/Serializer.h"
#include "Holloware/Scene/Scene.h"
#include "Holloware/Scene/Components.h"

#include <imgui_internal.h>

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

	UUID Entity::GetUUID() { return GetComponent<IDComponent>().ID; }
	bool Entity::operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool Entity::operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	void Entity::DrawGui()
	{
		const std::string& label = (*this) ? GetComponent<TagComponent>().Tag : "NULL";
		ImGui::Button(label.c_str(), { 200, 20 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
			{
				Entity* entity = (Entity*)payload->Data;
				*this = *entity;
			}
			ImGui::EndDragDropTarget();
		}
	}

	void Entity::Serialize(Serializer& serializer)
	{
		serializer.Add<uint64_t>(GetComponent<IDComponent>().ID, "ID");
		serializer.Add<std::string>(GetComponent<TagComponent>().Tag, "Tag");

		if (HasComponent<TransformComponent>())
			serializer.Add<HollowareObject*>(&GetComponent<TransformComponent>(), "TransformComponent");
		if (HasComponent<SpriteRendererComponent>())
			serializer.Add<HollowareObject*>(&GetComponent<SpriteRendererComponent>(), "SpriteRendererComponent");
		if (HasComponent<CameraComponent>())
			serializer.Add<HollowareObject*>(&GetComponent<CameraComponent>(), "CameraComponent");
		if (HasComponent<PythonScriptComponent>())
			serializer.Add<HollowareObject*>(&GetComponent<PythonScriptComponent>(), "PythonScriptComponent");
	}

	void Entity::Deserialize(const Serializer& serializer)
	{
		GetComponent<IDComponent>().ID = serializer.GetValue<uint64_t>("ID");
		GetComponent<TagComponent>().Tag = serializer.GetValue<std::string>("Tag");

		if (serializer.Contains("TransformComponent"))
			serializer.Deserialize<HollowareObject>(AddComponent<TransformComponent>(), "TransformComponent");
		if (serializer.Contains("SpriteRendererComponent"))
			serializer.Deserialize<HollowareObject>(AddComponent<SpriteRendererComponent>(), "SpriteRendererComponent");
		if (serializer.Contains("CameraComponent"))
			serializer.Deserialize<HollowareObject>(AddComponent<CameraComponent>(), "CameraComponent");
		if (serializer.Contains("PythonScriptComponent"))
			serializer.Deserialize<HollowareObject>(AddComponent<PythonScriptComponent>(), "PythonScriptComponent");
	}
}