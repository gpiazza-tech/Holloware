#include "hwpch.h"
#include "GameObject.h"

namespace Holloware
{
	GameObject::GameObject(glm::mat4x4 modelMatrix, std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertexArray)
		: m_ModelMatrix(modelMatrix), m_Shader(shader), m_VertexArray(vertexArray)
	{
	}
}