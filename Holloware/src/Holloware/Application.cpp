#include "Application.h"

#include "Holloware/Events/ApplicationEvent.h"
#include "Holloware/Log.h"

namespace Holloware
{
	Application::Application()
	{
	}
	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		HW_TRACE(e.ToString());

		while (true);
	}
}