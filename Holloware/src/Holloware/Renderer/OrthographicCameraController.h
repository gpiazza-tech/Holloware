#pragma once

#include "Holloware/Renderer/OrthographicCamera.h"
#include "Holloware/Core/Timestep.h"

#include "Holloware/Events/ApplicationEvent.h"
#include "Holloware/Events/MouseEvent.h"

namespace Holloware
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep);
		void OnEvent(Event& e);

		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = glm::vec3();
		float m_CameraRotation = 0.0f;

		float m_CameraMoveSpeed = 5.0f;
		float m_CameraTiltSpeed = 180.0f;
	};
}