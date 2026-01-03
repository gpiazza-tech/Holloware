#include <hwpch.h>
#include "Holloware/Scene/Components.h"

#include "Holloware/Serialization/Serializer.h"

namespace Holloware
{
	void IDComponent::Serialize(Serializer& serializer)
	{
		serializer.Add<uint64_t>((uint64_t)ID, "ID");
	}

	void IDComponent::Deserialize(const Serializer& serializer)
	{
		uint64_t id = ID;
		serializer.Deserialize<uint64_t>(id, "ID");
		ID = id;
	}

	void TagComponent::Serialize(Serializer& serializer)
	{
		serializer.Add<std::string>(Tag, "Tag");
	}

	void TagComponent::Deserialize(const Serializer& serializer)
	{
		serializer.Deserialize<std::string>(Tag, "Tag");
	}

	void TransformComponent::Serialize(Serializer& serializer)
	{
		serializer.Add<glm::vec3>(Position, "Position");
		serializer.Add<glm::vec3>(Rotation, "Rotation");
		serializer.Add<glm::vec3>(Scale, "Scale");
	}

	void TransformComponent::Deserialize(const Serializer& serializer)
	{
		serializer.Deserialize<glm::vec3>(Position, "Position");
		serializer.Deserialize<glm::vec3>(Rotation, "Rotation");
		serializer.Deserialize<glm::vec3>(Scale, "Scale");
	}

	void SpriteRendererComponent::Serialize(Serializer& serializer)
	{
		serializer.Add<glm::vec4>(Color, "Color");
	}

	void SpriteRendererComponent::Deserialize(const Serializer& serializer)
	{
		serializer.Deserialize<glm::vec4>(Color, "Color");
	}

	void CameraComponent::Serialize(Serializer& serializer)
	{
		serializer.Add<HollowareObject*>(&Camera, "Camera");
		serializer.Add<bool>(FixedAspectRatio, "FixedAspectRatio");
		serializer.Add<bool>(Primary, "Primary");
	}

	void CameraComponent::Deserialize(const Serializer& serializer)
	{
		serializer.Deserialize<HollowareObject>(Camera, "Camera");
		serializer.Deserialize<bool>(FixedAspectRatio, "FixedAspectRatio");
		serializer.Deserialize<bool>(Primary, "Primary");
	}

	void PythonScriptComponent::Serialize(Serializer& serializer)
	{
		serializer.Add<std::string>(Filepath, "Filepath");
	}

	void PythonScriptComponent::Deserialize(const Serializer& serializer)
	{
		serializer.Deserialize<std::string>(Filepath, "Filepath");
	}
}