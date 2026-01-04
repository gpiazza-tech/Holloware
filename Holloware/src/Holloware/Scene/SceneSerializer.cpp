#include <hwpch.h>
#include "SceneSerializer.h"

#include "Holloware/Serialization/Serializer.h"
#include "Holloware/Scene/Scene.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Scene/Components.h"

#include <glm/glm.hpp>
#include <fstream>

using json = nlohmann::json;

namespace Holloware
{
	bool SceneSerializer::Serialize(const Ref<Scene>& scene, const std::filesystem::path& path)
	{
		HW_PROFILE_FUNCTION();

		Serializer serializer = Serializer();

		for (auto entityID : scene->m_Registry.view<entt::entity>())
		{
			Entity entity = Entity(entityID, scene.get());
			std::string IDStr = std::string("Entity ").append(std::to_string(entity.GetComponent<IDComponent>().ID));
			serializer.Add<HollowareObject*>(&entity, IDStr.c_str());
		}

		serializer.WriteToFile(path);

		return false;
	}   

	Ref<Scene> SceneSerializer::Deserialize(const std::filesystem::path& path)
	{
		HW_PROFILE_FUNCTION();

		Serializer serializer = Serializer::LoadFromFile(path);
		Ref<Scene> scene = CreateRef<Scene>();

		for (auto& entityJson : serializer.GetJson())
		{
			serializer.Deserialize<HollowareObject>(scene->CreateAbstractEntity(), entityJson);
		}

		return scene;
	}
}