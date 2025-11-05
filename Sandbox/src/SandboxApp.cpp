#include <Holloware.h>
#include <Holloware/Core/EntryPoint.h>

#include <Platform/OpenGL/OpenGLShader.h>
#include "../../Holloware/vendor/imgui/imgui.h"

#include "Sandbox2D.h"

class ExampleLayer : public Holloware::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f), m_SquarePosition(0.0f)
	{
		
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	}

	void OnUpdate(Holloware::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Holloware::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}
private:
	Holloware::ShaderLibrary m_ShaderLibrary;

	Holloware::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition;
	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
};

class Sandbox : public Holloware::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{
	}
};

Holloware::Application* Holloware::CreateApplication() 
{
	return new Sandbox();
}