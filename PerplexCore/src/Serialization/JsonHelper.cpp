#include <Perplex/pch.h>
#include <Perplex/Serialization/JsonHelper.h>

#include <Perplex/Core/Log.h>

#include <nlohmann/json_fwd.hpp>

#include <filesystem>
#include <fstream>

namespace Perplex
{
	void JsonHelper::WriteToFile(const nlohmann::json& json, const std::filesystem::path& path) noexcept
	{
		std::ofstream output(path.string());
		output << json.dump(1);
		output.close();
	}

	nlohmann::json JsonHelper::LoadFromFile(const std::filesystem::path& path) noexcept
	{
		std::ifstream jsonFile(path.string());
		if (!jsonFile)
		{
			HW_CORE_WARN("file {0} does not exist, returning empty json.", path.string());
			return nlohmann::json{};
		}

		//if (!nlohmann::json::accept(jsonFile))
		//{
		//	HW_CORE_WARN("file {0} does not contain valid json, returning empty json.", path.string());
		//	return nlohmann::json{};
		//}

		return nlohmann::json::parse(jsonFile);
	}
}