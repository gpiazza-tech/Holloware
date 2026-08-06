#pragma once

#include <Perplex/Perplex.h>
#include <Perplex/ImGui/StringPopup.h>
#include <Perplex/ImGui/BoolPopup.h>
#include <pxr/pxr.h>

#include <filesystem>
#include <functional>

namespace Perplex
{
	class Texture2D;

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void SetOpenFileCallback(const std::function<void(const std::filesystem::path& path)>& callback) { m_OpenFileCallback = callback; }

		void OnImGuiRender();
	private:
		void RenderContent(const std::filesystem::path& assetDir);
	private:
		Ref<pxr::TextureBuffer> m_DirectoryIcon;
		Ref<pxr::TextureBuffer> m_FileIcon;
		Ref<pxr::TextureBuffer> m_BackArrowIcon;

		StringPopup m_RenameFilePopup;
		BoolPopup m_DeleteFilePopup;

		std::function<void(const std::filesystem::path& path)> m_OpenFileCallback{};
	};
}