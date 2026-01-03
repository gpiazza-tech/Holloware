#pragma once

#include <nlohmann/json_fwd.hpp>

namespace Holloware
{
	class Serializer;

	class HollowareObject
	{
	public:
		virtual void DrawGui() {}

		virtual void Serialize(Serializer& serializer);
		virtual void Deserialize(const Serializer& serializer);
	};
}