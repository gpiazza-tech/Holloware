#include "Sandbox2D.h"

#include "../../Holloware/vendor/imgui/imgui.h"
#include <iostream>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	HW_PROFILE_FUNCTION();

	m_FaceTexture = Holloware::Texture2D::Create("assets/textures/face.png");
	m_CheckerboardTexture = Holloware::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	HW_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Holloware::Timestep ts)
{
	HW_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	{
		HW_PROFILE_SCOPE("Renderer Prep");
		Holloware::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Holloware::RenderCommand::Clear();
	}

	{
		HW_PROFILE_SCOPE("Renderer Draw");
		Holloware::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Holloware::Renderer2D::DrawTexture({ { 0.8f, 5.1f, }, { 2.9f, 6.3f, } }, m_CheckerboardTexture);
		Holloware::Renderer2D::DrawQuad({ { 0.0f, 0.0f, }, { 1.0f, 1.0f, }, m_SquareColor });
		Holloware::Renderer2D::DrawQuad({ { 2.0f, -1.0f, }, { 1.0f, 2.0f, }, m_SquareColor });
		Holloware::Renderer2D::DrawTexture({ { -0.8f, -0.1f, }, { 0.7f, 1.0f, } }, m_FaceTexture);
		Holloware::Renderer2D::DrawTexture({ { 0.8f, 5.1f, }, { 6.9f, 1.3f, } }, m_CheckerboardTexture);
		Holloware::Renderer2D::DrawTexture({ { 0.8f, -2.0f, }, { 0.1f, 0.1f, } }, m_FaceTexture);

		Holloware::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	HW_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms - ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Holloware::Event& e)
{
	m_CameraController.OnEvent(e);
}
