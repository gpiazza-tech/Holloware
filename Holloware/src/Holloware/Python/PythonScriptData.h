#pragma once

#include "Holloware/Python/Attribute.h"

namespace Holloware
{
	struct PythonScriptData
	{
		std::string Code;
		std::vector<Attribute> Attributes;
	};
}