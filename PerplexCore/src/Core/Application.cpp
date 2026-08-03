#include <Perplex/pch.h>
#include <Perplex/Core/Application.h>

#include <Perplex/Core/Core.h>
#include <Perplex/Core/Window.h>
#include <Perplex/Core/Timestep.h>
#include <Perplex/Debug/Instrumentor.h>
#include <Perplex/Events/Event.h>
#include <Perplex/Events/ApplicationEvent.h>
#include <Perplex/Scene/SceneRenderer.h>
#include <Perplex/ImGui/ImGuiLayer.h>
#include <Perplex/Assets/AssetManager.h>
#include <Perplex/Serialization/JsonHelper.h>
#include <Perplex/Platform/SystemUtils.h>
#include <Perplex/Core/Game.h>
#include <pxr/pxr.h>

#include <filesystem>
#include <functional>
#include <memory>
#include <string>

// TODO: move to WindowsWindow
#include <GLFW/glfw3.h>

namespace fs = std::filesystem;
const static fs::path s_EngineResPath{ Perplex::PerplexAppDataPath() / "engine/res" };

static void CopyResFolder()
{
	// Create Perplex App Data directory if it doesn't already exist
	std::error_code errorCode{};

	if (!s_EngineResPath.parent_path().empty())
		std::filesystem::create_directories(s_EngineResPath.parent_path(), errorCode);

	HW_CORE_ASSERT(!errorCode, "Error creating directories for engine res folder!");

	// copy res path if found
	fs::path resPath = std::filesystem::current_path() / "../PerplexCore/res";
	HW_CORE_ASSERT(fs::exists(resPath), "Res folder in PerplexCore not found!");

	fs::copy(resPath, s_EngineResPath,
		fs::copy_options::recursive | fs::copy_options::overwrite_existing);
}

namespace Perplex
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		HW_PROFILE_FUNCTION();

		HW_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Create window
		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Window->SetVSync(false);

		// Initialize Renderer and Resources
		CopyResFolder();
		pxr::SetResourceFolder(s_EngineResPath);
		pxr::Renderer::Init(16);

		// Create ImGui Layer
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	std::filesystem::path Application::EngineRes(const std::filesystem::path& relative) const
	{
		return s_EngineResPath / relative;
	}

	Application::~Application()
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
			(*--it)->OnDetach();

		SaveGame();
		AssetManager::Cleanup();
	}

	void Application::PushLayer(Layer* layer)
	{
		HW_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		HW_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		HW_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<WindowRefreshEvent>(BIND_EVENT_FN(OnWindowRefresh));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	void Application::LoadGame(const fs::path& gamePath)
	{
		JsonHelper::ObjectFromFile(m_Game, gamePath);

		m_Game.RootDirectory = gamePath.parent_path();
		m_Game.AssetsDirectory = m_Game.RootDirectory / "assets";
		m_Game.Title = gamePath.stem().string();

		AssetManager::Init();
	}

	void Application::SaveGame() const
	{
		JsonHelper::ObjectToFile(m_Game, m_Game.RootDirectory / (m_Game.Title + ".pxgame"));
	}

	void Application::Update()
	{
		float time = (float)glfwGetTime(); // Platform::GetTime
		Timestep timestep = (time - m_LastFrameTime) * m_Timescale;
		m_LastFrameTime = time;

		if (!m_Minimized)
		{
			{
				HW_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				HW_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
		}
	}

	void Application::Run()
	{
		HW_PROFILE_FUNCTION();

		while (m_Running)
		{
			HW_PROFILE_SCOPE("Run Loop");

			Update();
			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	float Application::GetTimescale() const
	{
		return m_Timescale;
	}

	void Application::SetTimescale(float timescale)
	{
		m_Timescale = timescale;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		HW_PROFILE_FUNCTION();

		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		HW_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		return false;
	}

	bool Application::OnWindowRefresh(WindowRefreshEvent& e)
	{
		Update();
		return false;
	}
}