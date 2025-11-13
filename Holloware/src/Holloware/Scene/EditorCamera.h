#pragma once

#include "Holloware/Renderer/Camera.h"
#include "Holloware/Core/Timestep.h"

#include "Holloware/Events/Event.h"
#include "Holloware/Events/MouseEvent.h"
#include "Holloware/Events/ApplicationEvent.h"

#include <glm/glm.hpp>

namespace Holloware
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera();
		~EditorCamera() = default;

		void OnUpdate(Timestep ts);

		void OnEvent(Event& event);
		inline bool OnMouseScrolled(MouseScrolledEvent& e);
		inline bool OnMouseMoved(MouseMovedEvent& e);
		void OnResize(float width, float height);

		glm::mat4 GetViewProjection() const { return m_Projection * m_View; }
	private:
		void EditorCamera::CalculateView();
		void EditorCamera::CalculateProjection();
	private:
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 0.0f;

		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 20.0f);
		float m_PanSpeed = 5.0f;

		glm::vec2 m_MousePositionDelta;
		glm::vec2 m_InitialMousePosition;
	};
}