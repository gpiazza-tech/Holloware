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
		HW_INFO("ExampleLayer::Update");
	}

	void OnEvent(Holloware::Event& event) override
	{
		HW_TRACE("{0}", event);
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