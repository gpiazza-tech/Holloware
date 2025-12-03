#include <hwpch.h>
#include "Components.h"

#include "Holloware/ImGui/ImGuiUtilities.h"
#include "Holloware/Python/PythonEntity.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Holloware
{
	void IDComponent::DrawGUI()
	{
		std::string tempString = std::to_string(ID);
		ImGui::Text(tempString.c_str());
	}

	void TagComponent::DrawGUI()
	{
		char buffer[32];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), Tag.c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		{
			Tag = std::string(buffer);
		}
	}

	void TransformComponent::DrawGUI()
	{
		ImGuiUtilities::DrawVec3Control("Position", Position);

		glm::vec3 rotation = glm::degrees(Rotation);
		ImGuiUtilities::DrawVec3Control("Rotation", rotation);
		Rotation = glm::radians(rotation);

		ImGuiUtilities::DrawVec3Control("Scale", Scale);
	}

	void SpriteRendererComponent::DrawGUI()
	{
		ImGui::ColorEdit4("Color", glm::value_ptr(Color));

		static std::string label = "None";
		ImGui::Button(label.c_str(), { 200, 20 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::string pathString = std::filesystem::path(path).string();
				label = pathString;

				// TODO: Method for getting already bound textures
				Ref<Texture2D> tex = Texture2D::Create(pathString);
				SubTexture = CreateRef<SubTexture2D>(tex, glm::vec2(0, 0), glm::vec2(1, 1));
			}
			ImGui::EndDragDropTarget();
		}
	}

	void CameraComponent::DrawGUI()
	{
		auto& camera = Camera;

		ImGui::Checkbox("Primary", &Primary);

		const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

		if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = currentProjectionTypeString = projectionTypeStrings[i];
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeString = projectionTypeStrings[i];
					camera.SetProjectionType((SceneCamera::ProjectionType)i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
		{
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);

			float orthoNear = camera.GetOrthographicNearClip();
			if (ImGui::DragFloat("Near Clip", &orthoNear))
				camera.SetOrthographicNearClip(orthoNear);

			float orthoFar = camera.GetOrthographicFarClip();
			if (ImGui::DragFloat("Far Clip", &orthoFar))
				camera.SetOrthographicFarClip(orthoFar);

			ImGui::Checkbox("Fixed Aspect", &FixedAspectRatio);
		}


		if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
		{
			float perspectiveFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
			if (ImGui::DragFloat("FOV", &perspectiveFOV))
				camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));

			float perspectiveNear = camera.GetPerspectiveNearClip();
			if (ImGui::DragFloat("Near Clip", &perspectiveNear))
				camera.SetPerspectiveNearClip(perspectiveNear);

			float perspectiveFar = camera.GetPerspectiveFarClip();
			if (ImGui::DragFloat("Far Clip", &perspectiveFar))
				camera.SetPerspectiveFarClip(perspectiveFar);
		}
	}

	void PythonScriptComponent::DrawGUI()
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

		py::dict attributes = Instance->GetPythonObject().attr("__dict__");

		for (auto& attribute : attributes)
		{
			// Name
			std::string objName = attribute.first.cast<std::string>();

			bool privateAttr = objName.c_str()[0] == '_' || objName == "position" || objName == "rotation" || objName == "scale" || objName == "transform";
			if (privateAttr) { continue; }

			ImGui::PushID(objName.c_str());

			ImGui::Text(objName.c_str());

			// Value
			auto& typeObj = attribute.second.get_type();
			py::str typeObjPyStr = typeObj.attr("__name__");
			std::string typeStr = typeObjPyStr.cast<std::string>();

			ImGui::SameLine(150.0f, 10.0f);

			if (typeStr == "float")
			{
				float val = attributes[attribute.first].cast<float>();
				ImGui::DragFloat("", &val, 0.1f);
				attributes[attribute.first] = val;
			}
			else if (typeStr == "int")
			{
				int val = attributes[attribute.first].cast<int>();
				ImGui::DragInt("", &val);
				attributes[attribute.first] = val;
			}
			else if (typeStr == "bool")
			{
				bool val = attributes[attribute.first].cast<bool>();
				ImGui::Checkbox("", &val);
				attributes[attribute.first] = val;
			}
			else if (typeStr == "Vec3")
			{
				glm::vec3& val = attributes[attribute.first].cast<glm::vec3>();
				float valArray[3] = { val.x, val.y, val.z };
				ImGui::DragFloat3("", valArray, 0.1f);
				val = glm::make_vec3(valArray);
			}
			else
			{
				ImGui::Text(typeStr.c_str());
			}

			ImGui::PopID();
		}

		ImGui::PopID();
	}
}