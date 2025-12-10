#pragma once

#include <glm/fwd.hpp>

namespace std { typedef basic_string<char> string; }

namespace Holloware
{
	class ImGuiUtilities 
	{
	public:
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	};
}