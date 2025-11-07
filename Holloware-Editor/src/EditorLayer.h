#pragma once

#include <Holloware.h>

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
		OrthographicCameraController m_CameraController;

		Ref<Texture2D> m_FaceTexture;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;

		Ref<SubTexture2D> m_Grass;

		Ref<SubTexture2D> m_KeySubTexture;
		glm::vec2 m_KeySubTextureCoords = { 6, 6 };
		glm::vec2 m_KeySubTextureSize = { 1, 1 };

		Ref<GameObject> m_Square;
		glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };

		Ref<FrameBuffer> m_FrameBuffer;

		float m_frameMS;
		float m_FPS;

		glm::vec2 m_ViewportSize;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};
	};
}