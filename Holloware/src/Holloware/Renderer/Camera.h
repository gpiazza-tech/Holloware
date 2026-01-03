#pragma once

#include "Holloware/Core/HollowareObject.h"

#include <glm/ext/matrix_float4x4.hpp>

namespace Holloware
{
	class Camera : public HollowareObject
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) { }

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}