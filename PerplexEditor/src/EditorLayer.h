#pragma once

#include <Perplex/Perplex.h>

#include "Panels/Dockspace.h"
#include "Panels/ProjectPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PerpixelPanel.h"
#include "Panels/ViewportPanel.h"
#include "DebugPanels/SpriteRegistryPanel.h"
#include <Perplex/ImGui/StringPopup.h>
#include <Perplex/Core/RenderSettings.h>
#include "Meta/UserData.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Perplex
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() {};

		virtual void OnAttach() override;
		virtual void OnDetach() override;


		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		void OnScenePlay();
		void OnSceneStop();

		void OnResize();

		// UI Panels
		void UI_MenuBar();

		void UI_Stats();
		void UI_Toolbar();

		void OnAssetImported(Asset asset);
		void OnFileOpen(const std::filesystem::path& path);
	private:
		Ref<pxr::TextureBuffer> m_PlayIcon{};
		Ref<pxr::TextureBuffer> m_StopIcon{};

		float m_FrameMS = 0.0f;
		float m_FPS = 0.0f;

		SceneRenderer m_SceneRenderer{ 1, 1, 16 };
		AudioEngine m_AudioEngine{};

		EditorCamera m_EditorCamera{};
		
		// Panels
		Dockspace m_Dockspace{};
		ProjectPanel m_ProjectPanel{};
		ViewportPanel m_ViewportPanel{};
		SceneHierarchyPanel m_SceneHierarchyPanel{};
		ContentBrowserPanel m_ContentBrowserPanel{};
		PerpixelPanel m_PerpixelPanel{};

		// DebugPanels
		SpriteRegistryPanel m_SpriteRegistryPanel{};

		StringPopup m_SavePopup{ "Save Scene" };
		StringPopup m_LoadPopup{ "Load Scene" };

		// Meta
		UserData m_UserData{};

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		SceneState m_SceneState = SceneState::Edit;
	};
}