#pragma once

#include <memory>

#include "Core.h"

#include "Holloware/Core/Window.h"
#include "Holloware/Core/LayerStack.h"
#include "Holloware/Events/Event.h"
#include "Holloware/Events/ApplicationEvent.h"

#include "Holloware/Core/Timestep.h"

#include "Holloware/ImGui/ImGuiLayer.h"

namespace Holloware
{
	class HOLLOWARE_API Application
	{
	public:
		Application(const std::string& name = "Holloware App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

