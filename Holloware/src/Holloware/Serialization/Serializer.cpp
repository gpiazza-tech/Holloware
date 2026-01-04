#include <hwpch.h>
#include "Serializer.h"

#include "Holloware/Core/HollowareTypes.h"
#include "Holloware/Core/HollowareObject.h"

namespace Holloware
{
	void Serializer::WriteToFile(const std::filesystem::path& path)
	{
		std::ofstream output(path.string());
		output << m_Json.dump(1);
		output.close();
	}

	Serializer Serializer::LoadFromFile(const std::filesystem::path& path)
	{
		std::ifstream jsonFile(path.string());
		if (!jsonFile)
			HW_CORE_ERROR("file {0} does not exist!", path.string());

		json j = json::parse(jsonFile);
		jsonFile.close();

		return Serializer(j);
	}
}