#pragma once

#include "Holloware/Scene/Components.h"

#include <glm/glm.hpp>

using json = nlohmann::json;

namespace Holloware
{
	class Scene;
	class SceneSerializer
	{
	public:
		static bool Serialize(const Ref<Scene>& scene, const std::string& filepath);
		static Ref<Scene> Deserialize(const std::string& filepath);
	};
} 