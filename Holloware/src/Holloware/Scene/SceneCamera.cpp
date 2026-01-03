#include "hwpch.h"
#include "SceneCamera.h"

#include "Holloware/Serialization/Serializer.h"

#include <glm/ext/matrix_clip_space.hpp>

namespace Holloware
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}
	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}
	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}
	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
		else
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
	}

	void SceneCamera::Serialize(Serializer& serializer)
	{
		serializer.Add<int>((int)m_ProjectionType, "m_ProjectionType");

		serializer.Add<float>(m_OrthographicSize, "m_OrthographicSize");
		serializer.Add<float>(m_OrthographicNear, "m_OrthographicNear");
		serializer.Add<float>(m_OrthographicFar, "m_OrthographicFar");

		serializer.Add<float>(m_PerspectiveFOV, "m_PerspectiveFOV");
		serializer.Add<float>(m_PerspectiveNear, "m_PerspectiveNear");
		serializer.Add<float>(m_PerspectiveFar, "m_PerspectiveFar");

		serializer.Add<float>(m_AspectRatio, "m_AspectRatio");
	}

	void SceneCamera::Deserialize(const Serializer& serializer)
	{
		int projectionTypeInt = (int)m_ProjectionType;
		serializer.Deserialize<int>(projectionTypeInt, "m_ProjectionType");
		m_ProjectionType = (ProjectionType)projectionTypeInt;

		serializer.Deserialize<float>(m_OrthographicSize, "m_OrthographicSize");
		serializer.Deserialize<float>(m_OrthographicNear, "m_OrthographicNear");
		serializer.Deserialize<float>(m_OrthographicFar, "m_OrthographicFar");

		serializer.Deserialize<float>(m_PerspectiveFOV, "m_PerspectiveFOV");
		serializer.Deserialize<float>(m_PerspectiveNear, "m_PerspectiveNear");
		serializer.Deserialize<float>(m_PerspectiveFar, "m_PerspectiveFar");

		serializer.Deserialize<float>(m_AspectRatio, "m_AspectRatio");
	}
}