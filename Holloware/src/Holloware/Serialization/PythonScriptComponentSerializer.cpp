#include <hwpch.h>

#include "Holloware/Scene/Components.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ns 
{
	void to_json(json& j, const Holloware::PythonScriptComponent& p) 
	{
		// j = json{ {"name", p.name}, {"address", p.address}, {"age", p.age} };
	}

	void from_json(const json& j, Holloware::PythonScriptComponent& p) 
	{
		
	}
}