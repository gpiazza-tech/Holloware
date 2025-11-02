#include <Holloware.h>

class ExampleLayer : public Holloware::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_TrianglePosition(0.0f), m_SquarePosition(0.0f)
	{
		m_Camera = Holloware::Camera(-1.6f, 1.6f, -0.9f, 0.9f);

		std::shared_ptr<Holloware::VertexArray> triangleVA;
		triangleVA.reset(Holloware::VertexArray::Create());

		float trianglVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Holloware::VertexBuffer> triangleVB;
		triangleVB.reset(Holloware::VertexBuffer::Create(trianglVertices, sizeof(trianglVertices)));
		Holloware::BufferLayout layout = {
			{ Holloware::ShaderDataType::Float3, "a_Position" },
			{ Holloware::ShaderDataType::Float4, "a_Color" }
		};
		triangleVB->SetLayout(layout);
		triangleVA->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Holloware::IndexBuffer> triangleIB;
		triangleIB.reset(Holloware::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		triangleVA->SetIndexBuffer(triangleIB);

		std::string triangleVertexSrc = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string triangleFragmentSrc = R"(
			#version 330 core
		
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		std::shared_ptr<Holloware::Shader> triangleShader;
		triangleShader.reset(Holloware::Shader::Create(triangleVertexSrc, triangleFragmentSrc));

		m_Triangle.reset(new Holloware::GameObject(glm::translate(glm::mat4(1.0f), m_TrianglePosition), triangleShader, triangleVA));

		///////////////////

		std::shared_ptr<Holloware::VertexArray> squareVA;
		squareVA.reset(Holloware::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f
		};

		std::shared_ptr<Holloware::VertexBuffer> squareVB;
		squareVB.reset(Holloware::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Holloware::ShaderDataType::Float3, "a_Position" }
			});
		squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Holloware::IndexBuffer> squareIB;
		squareIB.reset(Holloware::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		squareVA->SetIndexBuffer(squareIB);

		std::string squareVertexSrc = R"(
			#version 330 core
		
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core
		
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.4, 0.65, 1.0);
			}
		)";

		std::shared_ptr<Holloware::Shader> squareShader;
		squareShader.reset(Holloware::Shader::Create(squareVertexSrc, squareFragmentSrc));

		m_Square.reset(new Holloware::GameObject(glm::translate(glm::mat4(1.0f), m_SquarePosition), squareShader, squareVA));
	}

	void OnUpdate(Holloware::Timestep ts) override
	{
		/////////////////////////////////////////////////////////////////////////////

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
		m_Triangle->SetPosition(m_TrianglePosition);

		Holloware::Renderer::Submit(m_Square->GetShader(), m_Square->GetVertexArray(), m_Square->GetModelMatrix());
		Holloware::Renderer::Submit(m_Triangle->GetShader(), m_Triangle->GetVertexArray(), m_Triangle->GetModelMatrix());

		Holloware::Renderer::EndScene();
	}

	void OnEvent(Holloware::Event& event) override
	{
	}
private:
	Holloware::Camera m_Camera;

	std::shared_ptr<Holloware::GameObject> m_Triangle;
	std::shared_ptr<Holloware::GameObject> m_Square;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraTiltSpeed = 180.0f;

	glm::vec3 m_TrianglePosition;
	float m_TriangleMoveSpeed = 1.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;
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