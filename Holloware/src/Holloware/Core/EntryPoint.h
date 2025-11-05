#pragma once

#ifdef HW_PLATFORM_WINDOWS

extern Holloware::Application* Holloware::CreateApplication();

int main(int argc, char** argv)
{
	Holloware::Log::Init();

	HW_PROFILE_BEGIN_SESSION("Startup", "HollowareProfile-Startup.json");
	auto app = Holloware::CreateApplication();
	HW_PROFILE_END_SESSION();

	HW_PROFILE_BEGIN_SESSION("Runtime", "HollowareProfile-Runtime.json");
	app->Run();
	HW_PROFILE_END_SESSION();

	HW_PROFILE_BEGIN_SESSION("Startup", "HollowareProfile-Shutdown.json");
	delete app;
	HW_PROFILE_END_SESSION();
}

#endif
