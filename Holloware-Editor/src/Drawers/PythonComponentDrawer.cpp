#include <hwpch.h>
#include "Holloware/Scene/Components.h"

#include "Holloware/ImGui/ImGuiUtilities.h"
#include "Holloware/Python/PythonEntity.h"
#include "Holloware/Python/PythonAttribute.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/ImGui/Drawer.h"

#include <imgui/imgui.h>

namespace Holloware
{
	void PythonScriptComponent::DrawGui()
	{
		// Head
		std::string fileName = std::filesystem::path(Filepath).filename().string();
		ImGui::Text(fileName.c_str());

		ImGui::SameLine();
		ImGui::Button(Filepath.c_str(), { 200, 20 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::string pathString = std::filesystem::path(path).string();

				if (std::filesystem::path(pathString).extension() == ".py")
				{
					Filepath = pathString;
				}
				else { HW_CORE_ERROR("{0} is not a python script file!", pathString); }
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		if (!IsSerialized)
		{
			// Serialize
		}

		// Draw Attributes
		if (Instance == nullptr) return;

		ImGui::PushID(Instance);

		std::vector<PythonAttribute>& attributes = Instance->GetAttributes();

		for (auto& attribute : attributes)
		{
			attribute.DrawGui();
		}

		ImGui::PopID();
	}
}