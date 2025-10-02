#pragma once

#ifdef HW_PLATFORM_WINDOWS

extern Holloware::Application* Holloware::CreateApplication();

int main(int argc, char** argv)
{
	printf("Holloware Endinge! :D");
	auto app = Holloware::CreateApplication();
	app->Run();
	delete app;
}

#endif
