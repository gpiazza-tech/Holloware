#pragma once

#include <Perplex/Core/Application.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Debug/Instrumentor.h>

#include <filesystem>

#ifdef HW_PLATFORM_WINDOWS

extern Perplex::Application* Perplex::CreateApplication(const std::filesystem::path& gameDirectory);

std::filesystem::path GetTargetDirectory(int argc, char** argv)
{
	std::filesystem::path gameDirectory{};

	if (argc == 1)
		gameDirectory = std::filesystem::current_path();

	else if (argc == 2)
		gameDirectory = argv[1];

	else if (argc > 2)
		HW_CORE_ERROR("Too many arguments!");

	if (argc == 2 && !std::filesystem::is_directory(gameDirectory))
		HW_CORE_ERROR("Target must be a directory!");

	return gameDirectory;
}

int main(int argc, char** argv)
{
	Perplex::Log::Init();

	HW_PROFILE_BEGIN_SESSION("Startup", "profiling/HollowareProfile-Startup.json");
	auto app = Perplex::CreateApplication(GetTargetDirectory(argc, argv));
	HW_PROFILE_END_SESSION();

	HW_PROFILE_BEGIN_SESSION("Runtime", "profiling/HollowareProfile-Runtime.json");
	app->Run();
	HW_PROFILE_END_SESSION();

	HW_PROFILE_BEGIN_SESSION("Shutdown", "profiling/HollowareProfile-Shutdown.json");
	delete app;
	HW_PROFILE_END_SESSION();
}

#endif
