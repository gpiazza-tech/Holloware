#include <Holloware.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include "../../Holloware/vendor/imgui/imgui.h"

class ExampleLayer : public Holloware::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
	{
		Holloware::Ref<Holloware::VertexArray> squareVA;
		squareVA.reset(Holloware::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.75f, -0.75f, 0.0f,   0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f,   1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f,   1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f,   0.0f, 1.0f
		};

		Holloware::Ref<Holloware::VertexBuffer> squareVB;
		squareVB.reset(Holloware::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Holloware::ShaderDataType::Float3, "a_Position" },
			{ Holloware::ShaderDataType::Float2, "a_TexCoord" }
			});
		squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Holloware::Ref<Holloware::IndexBuffer> squareIB;
		squareIB.reset(Holloware::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		squareVA->SetIndexBuffer(squareIB);

		Holloware::Ref<Holloware::Shader> flatColorShader;
		flatColorShader.reset(Holloware::Shader::Create("assets/shaders/FlatColor.glsl"));

		Holloware::Ref<Holloware::Shader> textureShader;
		textureShader.reset(Holloware::Shader::Create("assets/shaders/Texture.glsl"));

		m_FaceTexture = Holloware::Texture2D::Create("assets/textures/face.png");

		std::dynamic_pointer_cast<Holloware::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Holloware::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

		m_Square.reset(new Holloware::GameObject(glm::translate(glm::mat4(1.0f), m_SquarePosition), textureShader, squareVA));
	}

	void OnUpdate(Holloware::Timestep ts) override
	{
		if (Holloware::Input::IsKeyPressed(HW_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		if (Holloware::Input::IsKeyPressed(HW_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Holloware::Input::IsKeyPressed(HW_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Holloware::Input::IsKeyPressed(HW_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		/////////////////////////////////////////////////////////////////////////////

		if (Holloware::Input::IsKeyPressed(HW_KEY_A))
			m_CameraRotation += m_CameraTiltSpeed * ts;

		if (Holloware::Input::IsKeyPressed(HW_KEY_D))
			m_CameraRotation -= m_CameraTiltSpeed * ts;

		/////////////////////////////////////////////////////////////////////////////

		if (Holloware::Input::IsKeyPressed(HW_KEY_J))
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;

		if (Holloware::Input::IsKeyPressed(HW_KEY_L))
			m_SquarePosition.x += m_SquareMoveSpeed * ts;

		if (Holloware::Input::IsKeyPressed(HW_KEY_I))
			m_SquarePosition.y += m_SquareMoveSpeed * ts;

		if (Holloware::Input::IsKeyPressed(HW_KEY_K))
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;

		/////////////////////////////////////////////////////////////////////////////

		Holloware::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Holloware::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Holloware::Renderer::BeginScene(m_Camera);

		m_Square->SetPosition(m_SquarePosition);

		std::dynamic_pointer_cast<Holloware::OpenGLShader>(m_Square->GetShader())->Bind();
		std::dynamic_pointer_cast<Holloware::OpenGLShader>(m_Square->GetShader())->UploadUniformFloat3("u_Color", m_SquareColor);

		m_FaceTexture->Bind();

		Holloware::Renderer::Submit(m_Square->GetShader(), m_Square->GetVertexArray(), m_Square->GetModelMatrix());

		Holloware::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Holloware::Event& event) override
	{
	}
private:
	Holloware::Camera m_Camera;

	Holloware::Ref<Holloware::GameObject> m_Square;

	Holloware::Ref<Holloware::Texture2D> m_FaceTexture;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraTiltSpeed = 180.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;
	glm::vec3 m_SquareColor = { 0.8f, 0.2f, 0.3f };
};

class Sandbox : public Holloware::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{
	}
};

Holloware::Application* Holloware::CreateApplication() 
{
	return new Sandbox();
}