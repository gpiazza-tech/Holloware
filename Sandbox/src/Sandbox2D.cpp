#include "Sandbox2D.h"

#include "../../Holloware/vendor/imgui/imgui.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Holloware::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Holloware::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Holloware::RenderCommand::Clear();

	Holloware::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Holloware::Renderer2D::DrawQuad({ 0.0f, 0.0f, }, { 1.0f, 1.0f, }, m_SquareColor);
	Holloware::Renderer2D::DrawQuad({ 0.8f, 0.1f, }, { 2.0f, 0.5f, }, 45, { 0.5f, 0.5f, 0.5f, 0.5f, });
	Holloware::Renderer2D::DrawQuad({ -0.8f, -0.1f, }, { 1.0f, 1.0f, }, 45, Holloware::Texture2D::Create("assets/textures/face.png"));
	Holloware::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f, }, Holloware::Texture2D::Create("assets/textures/Checkerboard.png"));

	Holloware::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Holloware::Event& e)
{
	m_CameraController.OnEvent(e);
}
