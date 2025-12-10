#pragma once

#include <Holloware.h>

#include "Panels/Dockspace.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"

#include <imgui/imgui.h>

namespace Holloware
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void OnScenePlay();
		void OnSceneStop();

		void OnResize();

		// UI Panels
		void UI_MenuBar();
		void UI_Viewport();
		void UI_Stats();
		void UI_Toolbar();
	private:
		Ref<Texture2D> m_FaceTexture;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;

		Ref<SubTexture2D> m_Grass;

		Ref<Texture2D> m_PlayIcon, m_StopIcon;

		Ref<SubTexture2D> m_KeySubTexture;
		glm::vec2 m_KeySubTextureCoords = { 6, 6 };
		glm::vec2 m_KeySubTextureSize = { 1, 1 };

		Ref<Framebuffer> m_Framebuffer;

		float m_frameMS;
		float m_FPS;

		Ref<Scene> m_ActiveScene;
		Entity m_PlayerEntity;
		Entity m_CoinEntity1, m_CoinEntity2;
		Entity m_CameraEntity;

		Entity m_HoveredEntity;

		bool m_ViewportFocused, m_ViewportHovered;
		ImVec2 m_ViewportPanelSize = ImVec2(1.0f, 1.0f);
		glm::vec2 m_ViewportSize = glm::vec2(1.0f, 1.0f);
		glm::vec2 m_ViewportBounds[2];

		EditorCamera m_EditorCamera;

		PythonBinder m_PythonBinder;
		
		// Panels
		Dockspace m_Dockspace;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		SceneState m_SceneState = SceneState::Edit;
	};
}