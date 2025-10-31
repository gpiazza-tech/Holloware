#pragma once

#include <memory>

#include "Core.h"

#include "Holloware/Window.h"
#include "Holloware/LayerStack.h"
#include "Holloware/Events/Event.h"
#include "Holloware/Events/ApplicationEvent.h"

#include "Holloware/ImGui/ImGuiLayer.h"

#include "Holloware/Renderer/Shader.h"
#include "Holloware/Renderer/Buffer.h"

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

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

