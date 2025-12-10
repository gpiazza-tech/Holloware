#include <hwpch.h>
#include "Components.h"

#include "Holloware/Python/PythonAttribute.h"
#include "Holloware/ImGui/ImGuiUtilities.h"
#include "Holloware/Python/PythonEntity.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace py = pybind11;

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
		
		Instance->UpdateAttributes();
		std::vector<PythonAttribute> attributes = Instance->GetAttributes();

		for (auto& attribute : attributes)
		{
			const std::string& name = attribute.GetName();
			const std::string& type = attribute.GetType();

			ImGui::PushID(name.c_str());

			ImGui::Columns(2, (const char*)0, false);
			ImGui::Text(name.c_str());
			ImGui::NextColumn();

			if (type == "float")
			{
				float val = attribute.GetValue<float>();
				ImGui::DragFloat("", &val, 0.1f, 0.0f, 0.0f, "%.2f");
				attribute.SetValue<float>(val);
			}
			else if (type == "int")
			{
				int val = attribute.GetValue<int>();
				ImGui::DragInt("", &val);
				attribute.SetValue<int>(val);
			}
			else if (type == "bool")
			{
				bool val = attribute.GetValue<bool>();
				ImGui::Checkbox("", &val);
				attribute.SetValue<bool>(val);
			}
			else if (type == "Vec3")
			{
				glm::vec3& val = attribute.GetValue<glm::vec3>();
				float valArray[3] = { val.x, val.y, val.z };
				ImGui::DragFloat3("", valArray, 0.1f, 0.0f, 0.0f, "%.2f" );
				val = glm::make_vec3(valArray);
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