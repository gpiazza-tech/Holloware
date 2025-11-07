#include "hwpch.h"
#include "OrthographicCameraController.h"

#include "Holloware/Core/Input.h"

namespace Holloware
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_ZoomLevel(1.0f), m_Rotation(rotation), m_AspectRatio(aspectRatio), m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		HW_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(HW_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		if (Input::IsKeyPressed(HW_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Input::IsKeyPressed(HW_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Input::IsKeyPressed(HW_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (m_Rotation)
		{
			if (Holloware::Input::IsKeyPressed(HW_KEY_A))
				m_CameraRotation += m_CameraTiltSpeed * ts;

			if (Holloware::Input::IsKeyPressed(HW_KEY_D))
				m_CameraRotation -= m_CameraTiltSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraMoveSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		HW_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HW_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HW_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	inline bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HW_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset();
		m_ZoomLevel = std::max(0.25f, std::min(20.0f, m_ZoomLevel));
		CalculateView();
		return false;
	}

	inline bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		HW_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}