#pragma once

#include "imgui/imgui.h"
#include <Holloware.h>

#include "Panels/SceneHierarchyPanel.h"

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
		Ref<Texture2D> m_FaceTexture;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;

		Ref<SubTexture2D> m_Grass;

		Ref<SubTexture2D> m_KeySubTexture;
		glm::vec2 m_KeySubTextureCoords = { 6, 6 };
		glm::vec2 m_KeySubTextureSize = { 1, 1 };

		Ref<FrameBuffer> m_FrameBuffer;

		float m_frameMS;
		float m_FPS;

		Ref<Scene> m_ActiveScene;
		Entity m_CameraEntity;

		bool m_ViewportFocused, m_ViewportHovered;
		ImVec2 m_ViewportPanelSize = ImVec2(0.0f, 0.0f);
		glm::vec2 m_ViewportSize;

		EditorCamera m_EditorCamera;
		
		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}