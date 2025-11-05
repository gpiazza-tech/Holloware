#pragma once

#include <Holloware.h>

class Sandbox2D : public Holloware::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Holloware::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Holloware::Event& e) override;
private:
	Holloware::OrthographicCameraController m_CameraController;

	Holloware::Ref<Holloware::Texture2D> m_FaceTexture;
	Holloware::Ref<Holloware::Texture2D> m_CheckerboardTexture;

	Holloware::Ref<Holloware::GameObject> m_Square;
	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;
};