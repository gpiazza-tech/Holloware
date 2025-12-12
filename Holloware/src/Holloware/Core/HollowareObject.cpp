#include <hwpch.h>
#include "HollowareObject.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Holloware
{
	void HollowareObject::Serialize(json&) const
	{
	}

	void HollowareObject::Deserialize(const json&)
	{
	}
}