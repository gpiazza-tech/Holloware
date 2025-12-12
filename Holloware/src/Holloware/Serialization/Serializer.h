#pragma once

#include <glm/fwd.hpp>
#include <nlohmann/json_fwd.hpp>

namespace Holloware
{
	enum class HollowareTypes : uint8_t;
	class HollowareObject;

	class Serializer
	{
	public:
		static void SerializeType(nlohmann::json& j, const void* value, HollowareTypes type);
		static void Serialize(nlohmann::json& j, const float& value);
		static void Serialize(nlohmann::json& j, const int& value);
		static void Serialize(nlohmann::json& j, const bool& value);
		static void Serialize(nlohmann::json& j, const glm::vec3& value);
		static void Serialize(nlohmann::json& j, const HollowareObject* value);

		static void DeserializeType(const nlohmann::json& j, void* value, HollowareTypes type);
		static void Deserialize(const nlohmann::json& j, float& value);
		static void Deserialize(const nlohmann::json& j, int& value);
		static void Deserialize(const nlohmann::json& j, bool& value);
		static void Deserialize(const nlohmann::json& j, glm::vec3& value);
		static void Deserialize(const nlohmann::json& j, HollowareObject* value);
	};
}