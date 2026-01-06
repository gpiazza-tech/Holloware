#pragma once

#include "Holloware/Core/HollowareObject.h"
#include "Holloware/Renderer/OrthographicCamera.h"
#include "Holloware/Renderer/SubTexture2D.h"
#include "Holloware/Renderer/Texture.h"
#include "Holloware/Core/Timestep.h"
#include "Holloware/Scene/SerializableValue.h"

#include "Holloware/Assets/Asset.h"

#include "Holloware/Scene/SceneCamera.h"
#include "Holloware/Core/UUID.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Holloware
{
	class SerializedField;

	struct IDComponent : public HollowareObject
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

		void DrawGui() override;
		void Serialize(Serializer& serializer) override;
		void Deserialize(const Serializer& serializer) override;
	};

	struct TagComponent : public HollowareObject
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) { }

		void DrawGui() override;
		void Serialize(Serializer& serializer) override;
		void Deserialize(const Serializer& serializer) override;
	};

	struct TransformComponent : public HollowareObject
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

		void DrawGui() override;
		void Serialize(Serializer& serializer) override;
		void Deserialize(const Serializer& serializer) override;
	};

	struct SpriteRendererComponent : public HollowareObject
	{
		glm::vec4 Color = glm::vec4(1.0f);
		Asset TextureAsset = Asset();

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}

		void DrawGui() override;
		void Serialize(Serializer& serializer) override;
		void Deserialize(const Serializer& serializer) override;
	};

	struct CameraComponent : public HollowareObject
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: Scene should be responsible for this
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void DrawGui() override;
		void Serialize(Serializer& serializer) override;
		void Deserialize(const Serializer& serializer) override;
	};

	class PythonEntity;
	struct PythonScriptComponent : public HollowareObject
	{
		std::string Filepath;
		PythonEntity* Instance;

		bool IsSerialized;
		// std::vector<SerializableValue<int>> SerializableValues;
		
		PythonScriptComponent()
			: Filepath("none"), Instance(nullptr), IsSerialized(false) // , SerializableValues() 
		{ }
		PythonScriptComponent(const PythonScriptComponent&) = default;
		PythonScriptComponent(std::string filepath)
			: Filepath(filepath), Instance(nullptr), IsSerialized(false) // , SerializableValues()
		{
			
		}

		void DrawGui() override;
		void Serialize(Serializer& serializer) override;
		void Deserialize(const Serializer& serializer) override;
	};
}