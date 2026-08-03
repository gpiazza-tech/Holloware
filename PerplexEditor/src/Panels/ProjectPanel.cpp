#include <pch.h>
#include "ProjectPanel.h"

#include <Meta/UserData.h>
#include <Perplex/Platform/SystemUtils.h>
#include <Perplex/Scene/SceneManager.h>

#include <imgui/imgui.h>

#include <future>

namespace Perplex
{
	bool ProjectPanel::OnImGuiRender(UserData& userData)
	{
		if (m_FocusTrigger)
		{
			m_FocusTrigger = false;
			ImGui::SetNextWindowFocus();
		}

        bool opened{ false };
        static int selectedProject{ -1 };

        ImGui::Begin("Project Manager");

        float xRegion = ImGui::GetContentRegionAvail().x / 2.0f;
        float yRegion = ImGui::GetContentRegionAvail().y;

        if (ImGui::BeginChild("ProjectListRegion", ImVec2(xRegion, yRegion), true))
        {
            for (size_t i = 0; i < userData.RecentProjects.size(); i++)
            {
                bool isSelected = (selectedProject == i);

                if (ImGui::Selectable(userData.RecentProjects.at(i).stem().string().c_str(), isSelected))
                    selectedProject = static_cast<int>(i);
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Display details for the active selection
        ImGui::BeginChild("ProjectDetailsRegion", ImVec2(0, 300), false);

        // File Dialogue
        if (ImGui::Button("Open New"))
            m_FilepathToOpen = std::async(&FileDialogue);
        if (m_FilepathToOpen.valid())
        {
            std::future_status status = m_FilepathToOpen.wait_for(std::chrono::seconds(0));

            if (status == std::future_status::ready)
                opened = TryOpenFile(userData, m_FilepathToOpen.get());
        }

        ImGui::Dummy(ImVec2{ 0.0f, ImGui::GetContentRegionAvail().y / 2.0f });

        if (selectedProject != -1)
        {
            ImGui::Text("Path: %s", userData.RecentProjects.at(selectedProject).string().c_str());
            if (ImGui::Button("Open"))
            {
                opened = true;
                m_CurrentProjectRoot = userData.RecentProjects.at(selectedProject);
            }
        }

        ImGui::EndChild();

        ImGui::End();

        return opened;
	}

    bool ProjectPanel::TryOpenFile(UserData& userData, const std::filesystem::path& filepath)
    {
        // check if filepath ends in .pxgame
        if (filepath.extension().string() != ".pxgame")
        {
            HW_CORE_WARN("Selected file does not end in .pxgame!");
            return false;
        }

        // add to recent projects if it doesn't already contain it
        if (std::find(userData.RecentProjects.begin(), userData.RecentProjects.end(), filepath) == userData.RecentProjects.end())
            userData.RecentProjects.emplace_back(filepath);

        m_CurrentProjectRoot = filepath;
        return true;
    }

	void ProjectPanel::Focus()
	{
		m_FocusTrigger = true;
	}
}