#pragma once

#include <Perplex/Core/LayerStack.h>
#include <Perplex/Events/ApplicationEvent.h>
#include <Perplex/Core/Game.h>

#include <memory>
#include <string>

namespace Perplex
{
	class Window;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class ImGuiLayer;
	class Layer;

	class Application
	{
	public:
		Application(const std::filesystem::path& gameDirectory, const std::string& name = "Perplex App");
		virtual ~Application();

		void LoadGame(const std::filesystem::path& gamePath);
		void SaveGame() const;
		const Game& GetGame() { return m_Game; }

		void Update();
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		std::filesystem::path EngineRes(const std::filesystem::path& relative) const;

		float GetTimescale() const;
		void SetTimescale(float timescale);

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowRefresh(WindowRefreshEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer = nullptr;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		float m_LastFrameTime{ 0.0f };
		float m_Timescale{ 1.0f };

		Game m_Game{};
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication(const std::filesystem::path& gameDirectory);
}

