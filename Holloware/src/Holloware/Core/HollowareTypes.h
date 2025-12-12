#pragma once

#include <cstdint>

namespace Holloware
{
	enum class HollowareTypes : uint8_t
	{
		None = 0,
		Int,
		Float,
		Bool,
		Vec3,
		HollowareObject
	};

	class HollowareTypesConversions
	{
	public:
		static HollowareTypes PyToHwType(const std::string& pyType)
		{
			if (pyType == "int")
				return HollowareTypes::Int;
			else if (pyType == "float")
				return HollowareTypes::Float;
			else if (pyType == "bool")
				return HollowareTypes::Bool;
			else if (pyType == "Vec3")
				return HollowareTypes::Vec3;
			else if (pyType == "Entity")
				return HollowareTypes::HollowareObject;
			else 
				return HollowareTypes::None;
		}
	};
}