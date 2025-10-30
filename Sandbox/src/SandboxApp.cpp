#include <Holloware.h>

class ExampleLayer : public Holloware::Layer 
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Holloware::Input::IsKeyPressed(HW_KEY_TAB))
			HW_INFO("Tab key pressed!");
	}

	void OnEvent(Holloware::Event& event) override
	{
		if (event.GetEventType() == Holloware::EventType::KeyPressed)
		{
			Holloware::KeyPressedEvent& e = (Holloware::KeyPressedEvent&)event;
			HW_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Holloware::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{
	}
};

Holloware::Application* Holloware::CreateApplication() 
{
	return new Sandbox();
}