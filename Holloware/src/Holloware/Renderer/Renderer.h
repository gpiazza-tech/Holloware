#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Holloware/Renderer/GameObject.h"

namespace Holloware
{
	class Renderer
	{
	public:
		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}