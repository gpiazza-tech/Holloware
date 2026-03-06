#pragma once

#include "Holloware/Scripting/ScriptProperty.h"

namespace Holloware
{
	struct ScriptData
	{
		std::string Source;
		std::vector<ScriptProperty> Properties;
	};
}