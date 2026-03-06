#pragma once

#include "Holloware/Core/HollowareTypes.h"
#include "Holloware/Core/HollowareObject.h"

namespace Holloware
{
	struct Attribute : HollowareObject
	{
		std::string name;
		void* default;
		HollowareTypes type;
	};
}