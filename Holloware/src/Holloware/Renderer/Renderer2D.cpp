#include <hwpch.h>
#include "Renderer2D.h"

namespace Holloware
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		HW_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		};

		Ref<VertexBuffer> vb = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		vb->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			});
		s_Data->QuadVertexArray->AddVertexBuffer(vb);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		Ref<IndexBuffer> ib = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(ib);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		HW_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HW_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const QuadProperties& data)
	{
		HW_PROFILE_FUNCTION();

		DrawTexture(data, *s_Data->WhiteTexture.get());
	}

	void Renderer2D::DrawRotatedQuad(const QuadProperties& data, float rotation)
	{
		HW_PROFILE_FUNCTION();

		DrawRotatedTexture(data, rotation, *s_Data->WhiteTexture.get());
	}

	void Renderer2D::DrawTexture(const QuadProperties& data, const Texture2D& texture)
	{
		HW_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", data.Color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", data.TilingFactor);
		texture.Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), data.Position) *
			glm::scale(glm::mat4(1.0f), { data.Scale.x, data.Scale.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedTexture(const QuadProperties& data, float rotation, const Texture2D& texture)
	{
		HW_PROFILE_FUNCTION();

		s_Data->TextureShader->SetFloat4("u_Color", data.Color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", data.TilingFactor);
		texture.Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), data.Position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { data.Scale.x, data.Scale.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}