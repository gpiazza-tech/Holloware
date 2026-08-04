#include <Perplex/pch.h>
#include <Perplex/Core/FileIO.h>

#include <filesystem>
#include <optional>
#include <string_view>
#include <system_error>

namespace fs = std::filesystem;

namespace Perplex
{
	std::optional<fs::path> FileIO::FindFileWithExtension(std::string_view extension, const std::filesystem::path& directory)
	{
		std::optional<fs::path> result = std::nullopt;

		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (entry.path().extension() == extension)
				result = entry.path();
		}

		return result;
	}

	bool FileIO::TryCreateDirectories(const std::filesystem::path& directory)
	{
		std::error_code errorCode{};

		if (!directory.parent_path().empty())
			std::filesystem::create_directories(directory.parent_path(), errorCode);

		return !errorCode.value();
	}
}