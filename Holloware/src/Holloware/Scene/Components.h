#pragma once

#include "Holloware/Renderer/OrthographicCamera.h"
#include "Holloware/Renderer/SubTexture2D.h"
#include "Holloware/Renderer/Texture.h"
#include "Holloware/Core/Timestep.h"

#include "SceneCamera.h"
#include "Holloware/Core/UUID.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Holloware
{
	struct Component
	{
		virtual void DrawGUI() {}
	};

	struct IDComponent : public Component
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

		void DrawGUI() override;
	};

	struct TagComponent : public Component
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) { }

		void DrawGUI() override;
	};

	struct TransformComponent : public Component
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Position(translation) { }

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		void DrawGUI() override;
	};

	struct SpriteRendererComponent : public Component
	{
		glm::vec4 Color = glm::vec4(1.0f);
		Ref<SubTexture2D> SubTexture;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

		void DrawGUI() override;
	};

	struct CameraComponent : public Component
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: Scene should be responsible for this
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void DrawGUI() override;
	};

	class PythonEntity;
	struct PythonScriptComponent : public Component
	{
		std::string Filepath;
		PythonEntity* Instance;
		
		PythonScriptComponent()
			: Filepath("none"), Instance(nullptr) { }
		PythonScriptComponent(const PythonScriptComponent&) = default;
		PythonScriptComponent(std::string filepath)
			: Filepath(filepath), Instance(nullptr)
		{
			
		}

		void DrawGUI() override;
	};
}