#include "hwpch.h"
#include "ContentBrowserPanel.h"

#include "Holloware/Renderer/Texture.h"

#include <imgui/imgui.h>

namespace Holloware
{
	// To be stored in Project object
	static const std::filesystem::path s_AssetsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetsPath)
	{
		m_DirectoryIcon = Texture2D::Create("assets/textures/folder_icon.png");
		m_FileIcon = Texture2D::Create("assets/textures/file_icon.png"); 
		m_BackArrowIcon = Texture2D::Create("assets/textures/back_arrow.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content");

		if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath))
		{
			ImTextureRef arrowTextureRef = ImTextureRef(m_BackArrowIcon->GetRendererID());

			if (ImGui::ImageButton("back", arrowTextureRef, { 20, 20 }, {0, 1}, {1, 0}))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		float iconSize = 100.0f;
		float padding = 10.0f;
		float cellSize = iconSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1) columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (directoryEntry.path().extension() == ".meta") continue;

			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetsPath);
			std::string filenameString = relativePath.filename().string();

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImTextureRef iconTextureRef = ImTextureRef(icon->GetRendererID());

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton(filenameString.c_str(), iconTextureRef, { iconSize, iconSize }, {0, 1}, {1, 0} );

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = path.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
				else
				{
					std::string command = "start " + path.string();
					std::system(command.c_str());
				}
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::End();
	}
}