#include "hwpch.h"
#include "EditorCamera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Holloware
{
	EditorCamera::EditorCamera()
	{
		m_InitialMousePosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		CalculateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsMouseButtonPressed(0))
		{
			m_Position += glm::vec3(m_MousePositionDelta.x, -m_MousePositionDelta.y, 0.0f) * ts.GetMilliseconds() * m_Position.z;
			m_MousePositionDelta = glm::vec2();
			CalculateView();
		}
	}

	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher = EventDispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(HW_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
		dispatcher.Dispatch<MouseMovedEvent>(HW_BIND_EVENT_FN(EditorCamera::OnMouseMoved));
	}

	inline bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Position.z -= e.GetYOffset();
		CalculateView();
		return false;
	}

	inline bool EditorCamera::OnMouseMoved(MouseMovedEvent& e)
	{
		m_MousePositionDelta = (m_InitialMousePosition - glm::vec2(Input::GetMouseX(), Input::GetMouseY())) * 0.001f;
		m_InitialMousePosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		return false;
	}

	void EditorCamera::OnResize(float width, float height)
	{
		if (width == 0.0f || height == 0.0f) return;

		m_AspectRatio = width / height;
		CalculateProjection();
	}

	void EditorCamera::CalculateView()
	{
		m_View = glm::inverse(glm::translate(glm::mat4(1.0f), m_Position) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	}

	void EditorCamera::CalculateProjection()
	{
		m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
	}
}