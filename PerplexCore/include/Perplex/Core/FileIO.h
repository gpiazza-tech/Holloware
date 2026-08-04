#pragma once

#include <optional>
#include <filesystem>
#include <string_view>

namespace Perplex
{
	class FileIO
	{
	public:
		static std::optional<std::filesystem::path> FindFileWithExtension(std::string_view extension, const std::filesystem::path& directory);
		static bool TryCreateDirectories(const std::filesystem::path& directory);
	};
}