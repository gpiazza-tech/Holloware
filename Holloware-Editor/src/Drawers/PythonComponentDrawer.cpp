#include <hwpch.h>
#include "Holloware/Scene/Components.h"

#include "Holloware/ImGui/ImGuiUtilities.h"
#include "Holloware/Python/PythonEntity.h"
#include "Holloware/Python/PythonAttribute.h"
#include "Holloware/Scene/Entity.h"

#include <imgui/imgui.h>

namespace Holloware
{
	void PythonScriptComponent::DrawGui()
	{
		// Head
		std::string fileName = std::filesystem::path(Filepath).filename().string();
		ImGui::Text(fileName.c_str());

		static std::string label = Filepath;
		ImGui::SameLine();
		ImGui::Button(label.c_str(), { 200, 20 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::string pathString = std::filesystem::path(path).string();

				if (std::filesystem::path(pathString).extension() == ".py")
				{
					label = pathString;
					Filepath = pathString;
				}
				else { HW_CORE_ERROR("{0} is not a python script file!", pathString); }
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		// Draw Attributes
		if (Instance == nullptr) return;

		ImGui::PushID(Instance);

		std::vector<PythonAttribute>& attributes = Instance->GetAttributes();

		for (auto& attribute : attributes)
		{
			const std::string& name = attribute.GetName();
			const std::string& type = attribute.GetType();

			ImGui::PushID(name.c_str());

			ImGui::Columns(2, (const char*)0, false);
			ImGui::Text(name.c_str());
			ImGui::NextColumn();

			if (type == "<class 'float'>")
			{
				float val = attribute.GetValue<float>();
				ImGui::DragFloat("", &val, 0.1f, 0.0f, 0.0f, "%.2f");
				attribute.SetValue<float>(val);
			}
			else if (type == "<class 'int'>")
			{
				int val = attribute.GetValue<int>();
				ImGui::DragInt("", &val);
				attribute.SetValue<int>(val);
			}
			else if (type == "<class 'bool'>")
			{
				bool val = attribute.GetValue<bool>();
				ImGui::Checkbox("", &val);
				attribute.SetValue<bool>(val);
			}
			else if (type == "<class 'hw.Vec3'>")
			{
				glm::vec3& val = attribute.GetValue<glm::vec3>();
				float valArray[3] = { val.x, val.y, val.z };
				ImGui::DragFloat3("", valArray, 0.1f, 0.0f, 0.0f, "%.2f");
				val = glm::make_vec3(valArray);
			}
			else if (type == "<class 'hw.Entity'>")
			{
				Entity& val = attribute.GetValue<Entity>();

				const std::string& label = (val) ? val.GetComponent<TagComponent>().Tag : "NULL";
				ImGui::Button(label.c_str(), { 200, 20 });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
					{
						Entity* entity = (Entity*)payload->Data;
						attribute.SetValue<Entity>(*entity);
					}
					ImGui::EndDragDropTarget();
				}
			}
			else
			{
				ImGui::Text(type.c_str());
			}

			ImGui::Columns(1);

			ImGui::PopID();
		}

		ImGui::PopID();
	}
}