#pragma once

#include "OrthographicCamera.h"
#include "Camera.h"

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Texture.h"
#include "SubTexture2D.h"

namespace Holloware
{
	struct QuadProperties
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec2 Scale = glm::vec3(1.0f);
		glm::vec4 Color = glm::vec4(1.0f);
		float TilingFactor = 1.0f;

		QuadProperties(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
			: Position({ position.x, position.y, 0.0f }), Scale(scale), Color(color) {
		}
		QuadProperties(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
			: Position(position), Scale(scale), Color(color)  {
		}
		QuadProperties(const glm::vec2& position, const glm::vec2& scale)
			: Position({ position.x, position.y, 0.0f }), Scale(scale), Color(glm::vec4(1.0f)) {
		}
		QuadProperties(const glm::vec3& position, const glm::vec2& scale)
			: Position(position), Scale(scale), Color(glm::vec4(1.0f)) {
		}
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const QuadProperties& data);
		static void DrawRotatedQuad(const QuadProperties& data, float rotation);
		static void DrawTexture(const QuadProperties& data, const Ref<SubTexture2D>& subTexture);
		static void DrawRotatedTexture(const QuadProperties& data, float rotation, const Ref<SubTexture2D>& subTexture);
		
		static void DrawQuad(const glm::mat4 transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4 transform, const Ref<SubTexture2D>& texture);
		static void DrawQuad(const glm::mat4 transform, const Ref<SubTexture2D>& subTexture, const glm::vec4& tint);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();
	private:
		static void StartBatch();

	};
}