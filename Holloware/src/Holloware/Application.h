#pragma once

#include "Core.h"

namespace Holloware
{
	class HOLLOWARE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

