#pragma once

#include "Holloware/Renderer/Shader.h"
#include "Holloware/Renderer/VertexArray.h"
#include <vector>

#include <glm/glm.hpp>

namespace Holloware
{
	class GameObject
	{
	public:
		GameObject() {}
		GameObject(glm::mat4x4 modelMatrix, std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertexArray);

		inline const glm::mat4x4 GetModelMatrix() const { return m_ModelMatrix; }
		inline const std::shared_ptr<Shader> GetShader() const { return m_Shader; }
		inline const std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }
	private:
		glm::mat4x4 m_ModelMatrix;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
	};
}