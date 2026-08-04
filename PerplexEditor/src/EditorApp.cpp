#include <pch.h>

#include <Perplex/Perplex.h>
#include <Perplex/Core/EntryPoint.h>

#include "EditorLayer.h"

#include <filesystem>

namespace Perplex
{
	class PerplexEditor : public Application
	{
	public:
		PerplexEditor(const std::filesystem::path& gameDirectory)
			: Application(gameDirectory, "Perplex Editor")
		{
			PushLayer(new EditorLayer());
		}
		~PerplexEditor()
		{
		}
	};

	Application* CreateApplication(const std::filesystem::path& gameDirectory)
	{
		return new PerplexEditor(gameDirectory);
	}
}