#include <hwpch.h>
#include "Serializer.h"

#include "Holloware/Core/HollowareTypes.h"
#include "Holloware/Core/HollowareObject.h"

namespace Holloware
{
	void Serializer::WriteToFile(const std::string& filepath)
	{
		std::ofstream output(filepath);
		output << m_Json.dump(1);
		output.close();
	}

	Serializer Serializer::LoadFromFile(const std::string& filepath)
	{
		std::ifstream jsonFile(filepath);
		if (!jsonFile)
			HW_CORE_ERROR("file {0} does not exist!", filepath);

		json j = json::parse(jsonFile);
		jsonFile.close();

		return Serializer(j);
	}
}