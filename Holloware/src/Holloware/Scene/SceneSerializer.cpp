#include <hwpch.h>
#include "SceneSerializer.h"

#include "Scene.h"
#include "Entity.h"

#include <nlohmann/json.hpp>
#include <fstream>

namespace Holloware
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TransformComponent, Position, Rotation, Scale)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteRendererComponent, Color)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraComponent, Camera, FixedAspectRatio, Primary)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PythonScriptComponent, Filepath)

	bool SceneSerializer::Serialize(const Ref<Scene>& scene, const std::string& filepath)
	{
		json sceneJson = json();

		for (auto entityID : scene->m_Registry.view<entt::entity>())
		{
			Entity entity = Entity(entityID, scene.get());

			json entityJson;
			entityJson["Tag"] = entity.GetComponent<TagComponent>().Tag;
			entityJson["ID"] = (uint64_t)entity.GetComponent<IDComponent>().ID;
			entityJson["TransformComponent"] = entity.GetComponent<TransformComponent>();
			if (entity.HasComponent<SpriteRendererComponent>())
				entityJson["SpriteRendererComponent"] = entity.GetComponent<SpriteRendererComponent>();
			if (entity.HasComponent<CameraComponent>())
				entityJson["CameraComponent"] = entity.GetComponent<CameraComponent>();
			if (entity.HasComponent<PythonScriptComponent>())
				entityJson["PythonScriptComponent"] = entity.GetComponent<PythonScriptComponent>();

			sceneJson[entity.GetComponent<TagComponent>().Tag] = entityJson;
		}

		HW_CORE_INFO(sceneJson.dump(1));

		// Write to file
		std::ofstream output(filepath);
		output << sceneJson.dump(1);
		output.close();

		return false;
	}   

	Ref<Scene> SceneSerializer::Deserialize(const std::string& filepath)
	{
		Ref<Scene> scene = CreateRef<Scene>();

		// Read from file
		std::ifstream jsonFile(filepath);
		json data = json::parse(jsonFile);
		jsonFile.close();

		for (auto& entityJson : data)
		{
			Entity entity = scene->CreateEntity(entityJson.type_name());

			entity.GetComponent<TagComponent>().Tag = entityJson["Tag"].get<std::string>();
			entity.GetComponent<IDComponent>().ID = entityJson["ID"].get<uint64_t>();
			entity.GetComponent<TransformComponent>() = entityJson["TransformComponent"];
			if (entityJson.find("SpriteRendererComponent") != entityJson.end())
				entity.AddComponent<SpriteRendererComponent>() = entityJson["SpriteRendererComponent"];
			if (entityJson.find("CameraComponent") != entityJson.end())
			{
				entity.AddComponent<CameraComponent>() = entityJson["CameraComponent"];
				entity.GetComponent<CameraComponent>().Camera.RecalculateProjection();
			}
			if (entityJson.find("PythonScriptComponent") != entityJson.end())
				entity.AddComponent<PythonScriptComponent>() = entityJson["PythonScriptComponent"];
		}

		return scene;
	}
}

namespace glm
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec3, x, y, z)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec4, r, g, b, a)
}