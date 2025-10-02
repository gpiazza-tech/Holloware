#include <Holloware.h>

class Sandbox : public Holloware::Application
{
public:
	Sandbox()
	{
	}
	~Sandbox()
	{
	}
};

Holloware::Application* Holloware::CreateApplication() 
{
	return new Sandbox();
}