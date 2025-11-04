#include "Sandbox2D.h"

#include "../../Holloware/vendor/imgui/imgui.h"
#include <Platform/OpenGL/OpenGLShader.h>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
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

	m_Shader = Holloware::Shader::Create("assets/shaders/FlatColor.glsl");

	m_Square.reset(new Holloware::GameObject(glm::mat4(1.0f), m_Shader, squareVA));
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Holloware::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Holloware::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Holloware::RenderCommand::Clear();

	Holloware::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Holloware::OpenGLShader>(m_Shader)->Bind();
	std::dynamic_pointer_cast<Holloware::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Holloware::Renderer::Submit(m_Shader, m_Square->GetVertexArray(), m_Square->GetModelMatrix());

	Holloware::Renderer::EndScene();
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
