#include <pch.h>
#include "SpriteRegistryPanel.h"

#include <imgui/imgui.h>

#include <string>
#include <vector>

namespace Perplex
{
	void SpriteRegistryPanel::Render()
	{
		ImGui::Begin("Sprite Registry");

		std::vector<uint32_t> atlasIDs = pxr::SpriteRegistry::GetAtlasIDs();
        std::string selectedAtlasStr = std::to_string(m_SelectedAtlas);

        if (ImGui::BeginCombo("Atlas Index", selectedAtlasStr.c_str()))
        {
            for (size_t i{ 0 }; i < atlasIDs.size(); ++i)
            {
                const bool is_selected = (m_SelectedAtlas == i);

                std::string currentAtlasStr = std::to_string(i);

                if (ImGui::Selectable(currentAtlasStr.c_str(), is_selected))
                    m_SelectedAtlas = static_cast<uint32_t>(i);

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

		ImGui::Image(m_SelectedAtlas, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
}