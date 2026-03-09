#pragma once

#include "Holloware/Scripting/ScriptProperty.h"

namespace Holloware
{
	class Entity;

	struct ScriptData
	{
		std::string Source;
		std::vector<ScriptProperty> Properties;
	};
}