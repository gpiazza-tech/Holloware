#include <hwpch.h>
#include "ScriptProperty.h"

#include "Holloware/Serialization/Serializer.h"

#include "Holloware/Scene/Entity.h"
#include "Holloware/Assets/Asset.h"

#include "Holloware/ImGui/ImGuiUtilities.h"

#include <imgui.h>

namespace Holloware
{
	ScriptProperty::ScriptProperty(const std::string& name, const std::string& type, const std::string& strValue)
	{
		m_Name = name;
		m_Type = HollowareTypesConversions::CToHwType(type);

		if (strValue != "")
		{
			switch (m_Type)
			{
			case HollowareTypes::Int:
				m_Value = StringToValue<int>(strValue);
				break;
			case HollowareTypes::Float:
				m_Value = StringToValue<float>(strValue);
				break;
			case HollowareTypes::Double:
				m_Value = StringToValue<double>(strValue);
				break;
			case HollowareTypes::Bool:
				m_Value = StringToValue<bool>(strValue);
				break;
			}
		}

		else // default values
		{
			switch (m_Type)
			{
			case HollowareTypes::Int:
				m_Value = 0;
				break;
			case HollowareTypes::Float:
				m_Value = 0.0f;
				break;
			case HollowareTypes::Double:
				m_Value = 0.0;
				break;
			case HollowareTypes::Bool:
				m_Value = false;
				break;
			}
		}
	}

	void ScriptProperty::TrySync(const ScriptProperty& property)
	{
		if (property.m_Name == m_Name && property.m_Type == m_Type)
		{
			m_Value = property.m_Value;
		}
	}

	void ScriptProperty::DrawGui()
	{
		switch (m_Type)
		{
		case HollowareTypes::Int:
			ImGuiUtilities::DrawAnyIntControl(m_Name.c_str(), m_Value);
			break;
		case HollowareTypes::Float:
			ImGuiUtilities::DrawAnyFloatControl(m_Name.c_str(), m_Value);
			break;
		case HollowareTypes::Double:
			ImGuiUtilities::DrawAnyDoubleControl(m_Name.c_str(), m_Value);
 			break;
		case HollowareTypes::Bool:
			ImGuiUtilities::DrawAnyBoolControl(m_Name.c_str(), m_Value);
			break;
		case HollowareTypes::Vec3:
			ImGuiUtilities::DrawVec3Control(m_Name, std::any_cast<glm::vec3&>(m_Value));
			break;
		case HollowareTypes::Entity:
			ImGui::Text("Entity");
			break;
		case HollowareTypes::Asset:
			ImGui::Text("Asset");
			break;
		default:
			break;
		}
	}

	void ScriptProperty::Deserialize(const Serializer& serializer)
	{
		serializer.Deserialize(m_Name, "m_Name");
		serializer.Deserialize(m_Type, "m_Type");

		switch (m_Type)
		{
		case HollowareTypes::Int:
			serializer.Deserialize<int>(m_Value, "m_Value");
			break;
		case HollowareTypes::Float:
			serializer.Deserialize<float>(m_Value, "m_Value");
			break;
		case HollowareTypes::Double:
			serializer.Deserialize<double>(m_Value, "m_Value");
			break;
		case HollowareTypes::Bool:
			serializer.Deserialize<bool>(m_Value, "m_Value");
			break;
		case HollowareTypes::Vec3:
			serializer.Deserialize<glm::vec3>(m_Value, "m_Value");
			break;
		case HollowareTypes::Entity:
			serializer.Deserialize<uint64_t>(m_Value, "m_Value");
			break;
		case HollowareTypes::Asset:
			serializer.Deserialize<uint64_t>(m_Value, "m_Value");
			break;
		default:
			break;
		}
	}

	void ScriptProperty::Serialize(Serializer& serializer)
	{
		serializer.Add(m_Name, "m_Name");
		serializer.Add(m_Type, "m_Type");

		switch (m_Type)
		{
		case HollowareTypes::Int:
			serializer.Add<int>(std::any_cast<int>(m_Value), "m_Value");
			break;
		case HollowareTypes::Float:
			serializer.Add<float>(std::any_cast<float>(m_Value), "m_Value");
			break;
		case HollowareTypes::Double:
			serializer.Add<double>(std::any_cast<double>(m_Value), "m_Value");
			break;
		case HollowareTypes::Bool:
			serializer.Add<bool>(std::any_cast<bool>(m_Value), "m_Value");
			break;
		case HollowareTypes::Vec3:
			serializer.Add<glm::vec3>(std::any_cast<glm::vec3>(m_Value), "m_Value");
			break;
		case HollowareTypes::Entity:
			serializer.Add<uint64_t>(std::any_cast<Entity>(m_Value).GetUUID(), "m_Value");
			break;
		case HollowareTypes::Asset:
			serializer.Add<uint64_t>(std::any_cast<Asset>(m_Value), "m_Value");
			break;
		default:
			break;
		}
	}
}