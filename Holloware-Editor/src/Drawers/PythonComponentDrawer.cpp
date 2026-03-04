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
		ImGui::PushID(this);

		// Head
		std::string fileName = ScriptAsset.GetPath().filename().string();
		ImGui::Text(fileName.c_str());

		ImGui::SameLine();
		std::string assetPathString = ScriptAsset.GetPath().string();
		ImGui::Button(assetPathString.c_str(), { 200, 20 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				Asset asset = *(Asset*)payload->Data;
				std::string pathString = asset.GetPath().string();

				if (std::filesystem::path(pathString).extension() == ".py")
				{
					ScriptAsset = asset;
					std::string pyScriptName = ScriptAsset.GetPath().stem().string();
					// TODO: PythonBinder::BindEntityToScript
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

		ImGui::PopID();

		// Draw Attributes
		if (Instance == nullptr) return;

		ImGui::PushID(Instance);

		Instance->DrawGui();

		ImGui::PopID();
	}
}