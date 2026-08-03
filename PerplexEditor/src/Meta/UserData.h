#pragma once

#include <Perplex/Serialization/JsonHelper.h>

#include <vector>
#include <filesystem>

namespace Perplex
{
	struct UserData
	{
		std::vector<std::filesystem::path> RecentProjects{};
	};

	PERPLEX_DEFINE_JSON_STRUCT(UserData, RecentProjects)
}