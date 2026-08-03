#pragma once

#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Core/UUID.h>

namespace Perplex
{
	struct Game
	{
		std::filesystem::path RootDirectory{};
		std::filesystem::path AssetsDirectory{};

		std::string Title{ "Untitled" };

		UUID StartScene{ 0 };
	};

	PERPLEX_DEFINE_JSON_STRUCT(Game, RootDirectory, AssetsDirectory, StartScene)
}