#include <hwpch.h>
#include "PythonAttribute.h"

#include "Holloware/ImGui/Drawer.h"
#include <Holloware/Scene/Entity.h>

#include <imgui_internal.h>

namespace Holloware
{
	void PythonAttribute::DrawGui()
	{
		const std::string& name = GetName();
		const std::string& type = GetType();

		ImGui::PushID(name.c_str());

		ImGui::Columns(2, (const char*)0, false);
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		switch (m_Type)
		{
		case Holloware::HollowareTypes::Int:
		{
			int intVal = GetValue<int>();
			Drawer::DrawType(&intVal, m_Type);
			SetValue<int>(intVal);
			break;
		}
		case Holloware::HollowareTypes::Float:
		{
			float floatVal = GetValue<float>();
			Drawer::DrawType(&floatVal, m_Type);
			SetValue<float>(floatVal);
			break;
		}
		case Holloware::HollowareTypes::Bool:
		{
			bool boolVal = GetValue<bool>();
			Drawer::DrawType(&boolVal, m_Type);
			SetValue<bool>(boolVal);
			break;
		}
		case Holloware::HollowareTypes::Vec3:
		{
			glm::vec3& vec3Val = GetValue<glm::vec3>();
			Drawer::DrawType(&vec3Val, m_Type);
			SetValue<glm::vec3>(vec3Val);
			break;
		}
		case Holloware::HollowareTypes::HollowareObject:
		{
			Entity& hwObjVal = GetValue<Entity>();
			Drawer::DrawType(&hwObjVal, m_Type);
			SetValue<Entity>(hwObjVal);
			break;
		}
		default:
			ImGui::Text("{0}", m_Type);
			break;
		}

		ImGui::Columns(1);
		ImGui::PopID();
	}
}