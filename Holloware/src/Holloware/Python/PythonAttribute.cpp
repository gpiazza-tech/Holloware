#include <hwpch.h>
#include "PythonAttribute.h"

#include <Holloware/Core/HollowareTypes.h>
#include "Holloware/Serialization/Serializer.h"
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
		case HollowareTypes::Int:
		{
			int intVal = GetValue<int>();
			Drawer::DrawType(&intVal, m_Type);
			SetValue<int>(intVal);
			break;
		}
		case HollowareTypes::Float:
		{
			float floatVal = GetValue<float>();
			Drawer::DrawType(&floatVal, m_Type);
			SetValue<float>(floatVal);
			break;
		}
		case HollowareTypes::Bool:
		{
			bool boolVal = GetValue<bool>();
			Drawer::DrawType(&boolVal, m_Type);
			SetValue<bool>(boolVal);
			break;
		}
		case HollowareTypes::Vec3:
		{
			glm::vec3& vec3Val = GetValue<glm::vec3>();
			Drawer::DrawType(&vec3Val, m_Type);
			SetValue<glm::vec3>(vec3Val);
			break;
		}
		case HollowareTypes::HollowareObject:
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

	void PythonAttribute::Serialize(Serializer& serializer)
	{
		int typeInt = (int)m_Type;
		serializer.Add(typeInt, "m_Type");

		switch (m_Type)
		{
		case HollowareTypes::Int:
		{
			int intVal = GetValue<int>();
			serializer.Add(intVal, GetName().c_str());
			break;
		}
		case HollowareTypes::Float:
		{
			float floatVal = GetValue<float>();
			serializer.Add(floatVal, GetName().c_str());
			break;
		}
		case HollowareTypes::Bool:
		{
			bool boolVal = GetValue<bool>();
			serializer.Add(boolVal, GetName().c_str());
			break;
		}
		case HollowareTypes::Vec3:
		{
			glm::vec3& vec3Val = GetValue<glm::vec3>();
			serializer.Add(vec3Val, GetName().c_str());
			break;
		}
		case HollowareTypes::HollowareObject:
		{
			Entity& hwObjVal = GetValue<Entity>();
			serializer.Add<HollowareObject*>(&hwObjVal, GetName().c_str());
			break;
		}
		default:
			HW_CORE_WARN("Unsupported type for serialization: {0}", (int)m_Type);
			break;
		}
	}

	void PythonAttribute::Deserialize(const Serializer& serializer)
	{
		int typeInt = 0;
		serializer.Deserialize(typeInt, "m_Type");
		m_Type = (HollowareTypes)typeInt;

		switch (m_Type)
		{
		case HollowareTypes::Int:
		{
			int intVal = GetValue<int>();
			serializer.Deserialize(intVal, GetName().c_str());
			SetValue<int>(intVal);
			break;
		}
		case HollowareTypes::Float:
		{
			float floatVal = GetValue<float>();
			serializer.Deserialize(floatVal, GetName().c_str());
			SetValue<float>(floatVal);
			break;
		}
		case HollowareTypes::Bool:
		{
			bool boolVal = GetValue<bool>();
			serializer.Deserialize(boolVal, GetName().c_str());
			SetValue<bool>(boolVal);
			break;
		}
		case HollowareTypes::Vec3:
		{
			glm::vec3& vec3Val = GetValue<glm::vec3>();
			serializer.Deserialize(vec3Val, GetName().c_str());
			SetValue<glm::vec3>(vec3Val);
			break;
		}
		case HollowareTypes::HollowareObject:
		{
			Entity& hwObjVal = GetValue<Entity>();
			serializer.Deserialize<HollowareObject>(hwObjVal, GetName().c_str());
			SetValue<Entity>(hwObjVal);
			break;
		}
		default:
			HW_CORE_WARN("Unsupported type for deserialization: {0}", (int)m_Type);
			break;
		}
	}
}