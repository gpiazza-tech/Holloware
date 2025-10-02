#pragma once

#ifdef HW_PLATFORM_WINDOWS

extern Holloware::Application* Holloware::CreateApplication();

int main(int argc, char** argv)
{
	Holloware::Log::Init();
	HW_CORE_WARN("Initialized Log!");
	int a = 5;
	HW_INFO("Hello! Var={0}", a);

	auto app = Holloware::CreateApplication();
	app->Run();
	delete app;
}

#endif
