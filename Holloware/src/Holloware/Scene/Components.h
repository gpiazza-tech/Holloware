#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Holloware
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) { }
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}
		TransformComponent(const glm::vec2& position)
			: Transform(glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f })) {
		}
		TransformComponent(const glm::vec2& position, const glm::vec2& scale)
			: Transform(glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) *
			glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 0.0f })) {
		}

		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};
}