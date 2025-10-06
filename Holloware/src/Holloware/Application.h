#pragma once

#include <memory>

#include "Core.h"

#include "Holloware/Window.h"
#include "Holloware/LayerStack.h"
#include "Holloware/Events/Event.h"
#include "Holloware/Events/ApplicationEvent.h"

namespace Holloware
{
	class HOLLOWARE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

