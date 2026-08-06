#include <pch.h>

#include <Perplex/Perplex.h>
#include <Perplex/Core/EntryPoint.h>

#include "RuntimeLayer.h"

#include <filesystem>

namespace Perplex
{
	class PerplexRuntime : public Application
	{
	public:
		PerplexRuntime(const std::filesystem::path& gameDirectory)
			: Application(gameDirectory, "Perplex Runtime")
		{
			PushLayer(new RuntimeLayer());
		}
		~PerplexRuntime()
		{
		}
	};

	Application* CreateApplication(const std::filesystem::path& gameDirectory)
	{
		return new PerplexRuntime(gameDirectory);
	}
}