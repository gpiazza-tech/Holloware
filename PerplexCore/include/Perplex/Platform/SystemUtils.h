#pragma once

#include <filesystem>

namespace Perplex
{
	std::filesystem::path NativePath(const std::filesystem::path& path);

	std::filesystem::path PerplexAppUserPath();
	std::filesystem::path PerplexAppDataPath();

	void OpenFilesystemGui(const std::filesystem::path& path);
	std::filesystem::path FileDialogue();

	float GetTime();
}