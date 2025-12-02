#pragma once

#include <string>
#include <glm/ext/vector_float3.hpp>

namespace Holloware
{
	class ImGuiUtilities 
	{
	public:
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	};
}