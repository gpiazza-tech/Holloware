#pragma once

namespace std { typedef basic_string<char> string; }

namespace Holloware
{
	class Scene;
	class SceneSerializer
	{
	public:
		static bool Serialize(const Ref<Scene>& scene, const std::filesystem::path& path);
		static Ref<Scene> Deserialize(const std::filesystem::path& path);
	};
} 