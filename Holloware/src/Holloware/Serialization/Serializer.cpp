#include <hwpch.h>
#include "Serializer.h"

#include "Holloware/Core/HollowareTypes.h"
#include "Holloware/Core/HollowareObject.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Holloware
{
	void Serializer::SerializeType(json& j, const void* value, HollowareTypes type)
	{
		switch (type)
		{
		case Holloware::HollowareTypes::Int:
			Serialize(j, *(int*)value);
			break;
		case Holloware::HollowareTypes::Float:
			Serialize(j, *(float*)value);
			break;
		case Holloware::HollowareTypes::Bool:
			Serialize(j, *(bool*)value);
			break;
		case Holloware::HollowareTypes::Vec3:
			Serialize(j, *(glm::vec3*)value);
			break;
		case Holloware::HollowareTypes::HollowareObject:
			Serialize(j, (HollowareObject*)value);
			break;
		default:
			HW_CORE_ASSERT(false, "HollowareType {0} not supported!", type);
		}
	}

	void Serializer::Serialize(json& j, const float& value)
	{
		j = value;
	}

	void Serializer::Serialize(json& j, const int& value)
	{
		j = value;
	}

	void Serializer::Serialize(json& j, const bool& value)
	{
		j = value;
	}

	void Serializer::Serialize(json& j, const glm::vec3& value)
	{
		j["x"] = value.x;
		j["y"] = value.y;
		j["z"] = value.z;
	}

	void Serializer::Serialize(json& j, const HollowareObject* value)
	{
		value->Serialize(j);
	}

	void Serializer::DeserializeType(const nlohmann::json& j, void* value, HollowareTypes type)
	{
		switch (type)
		{
		case Holloware::HollowareTypes::Int:
			Deserialize(j, *(int*)value);
			break;
		case Holloware::HollowareTypes::Float:
			Deserialize(j, *(float*)value);
			break;
		case Holloware::HollowareTypes::Bool:
			Deserialize(j, *(bool*)value);
			break;
		case Holloware::HollowareTypes::Vec3:
			Deserialize(j, *(glm::vec3*)value);
			break;
		case Holloware::HollowareTypes::HollowareObject:
			Deserialize(j, (HollowareObject*)value);
			break;
		default:
			HW_CORE_ASSERT(false, "HollowareType {0} not supported!", type);
		}
	}

	void Serializer::Deserialize(const nlohmann::json& j, float& value)
	{
		value = j;
	}

	void Serializer::Deserialize(const nlohmann::json& j, int& value)
	{
		value = j;
	}

	void Serializer::Deserialize(const nlohmann::json& j, bool& value)
	{
		value = j;
	}

	void Serializer::Deserialize(const nlohmann::json& j, glm::vec3& value)
	{
		value.x = j["x"];
		value.y = j["y"];
		value.z = j["z"];
	}

	void Serializer::Deserialize(const nlohmann::json& j, HollowareObject* value)
	{
		value->Deserialize(j);
	}
}