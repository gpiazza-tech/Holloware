#pragma once

#include <Perplex/Perplex.h>

#include <Meta/UserData.h>

#include <imgui/imgui.h>

#include <future>

namespace Perplex
{
	class ProjectPanel
	{
	public:
		bool OnImGuiRender(UserData& userData);
		void Focus();

		const std::filesystem::path& GetCurrentProjectRoot() { return m_CurrentProjectRoot; }
	private:
		bool TryOpenFile(UserData& userData, const std::filesystem::path& filepath);
	private:
		bool m_FocusTrigger{ false };
		std::future<std::filesystem::path> m_FilepathToOpen{};

		std::filesystem::path m_CurrentProjectRoot{};
	};
}