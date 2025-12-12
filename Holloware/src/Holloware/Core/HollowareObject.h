#pragma once

#include <nlohmann/json_fwd.hpp>

namespace Holloware
{
	class HollowareObject
	{
	public:
		virtual void DrawGui() {}

		virtual void Serialize(nlohmann::json&) const;
		virtual void Deserialize(const nlohmann::json&);
	};
}