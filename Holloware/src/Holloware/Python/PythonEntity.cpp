#include <hwpch.h>
#include "PythonEntity.h"

#include "Holloware/Core/Timestep.h"
#include "Holloware/Core/HollowareObject.h"
#include "Holloware/Python/PythonAttribute.h"
#include "Holloware/Serialization/Serializer.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace py = pybind11;

namespace Holloware
{
	PythonEntity::PythonEntity(const std::string& pyClassName, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
		: m_PyClassName(pyClassName), m_PyObject(py::object(py::globals()[pyClassName.c_str()]()))
	{
		BindTransform(position, rotation, scale);
		BindAttributes();
	}

	PythonEntity::~PythonEntity()
	{
		m_Attributes.clear();
		m_PyClassName.clear();
	}

	void PythonEntity::OnStart() { m_PyObject.attr("on_start")(); }
	void PythonEntity::OnUpdate(Timestep ts) { m_PyObject.attr("on_update")(ts); }
	void PythonEntity::OnDestroy() { m_PyObject.attr("on_destroy")(); }

	void PythonEntity::BindTransform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
	{
		m_PyObject.attr("position") = py::cast(&position, py::return_value_policy::reference);
		m_PyObject.attr("rotation") = py::cast(&rotation, py::return_value_policy::reference);
		m_PyObject.attr("scale") = py::cast(&scale, py::return_value_policy::reference);
	}

	void PythonEntity::BindAttributes()
	{
		HW_PROFILE_FUNCTION();

		py::dict pyAttributes = m_PyObject.attr("__dict__");

		m_Attributes = std::vector<PythonAttribute>();
		m_Attributes.reserve(py::len(pyAttributes));

		for (auto& attribute : pyAttributes)
		{
			// Name
			std::string objName = attribute.first.cast<std::string>();
			bool privateAttr = objName.c_str()[0] == '_' || objName == "position" || objName == "rotation" || objName == "scale" || objName == "transform" || objName == "on_start" || objName == "on_update" || objName == "on_destroy";
			if (privateAttr) { continue; }

			// Type
			auto& typeObj = attribute.second.get_type();
			py::str typeObjPyStr = typeObj.attr("__name__");
			std::string typeStr = typeObjPyStr.cast<std::string>();

			HollowareTypes type = HollowareTypesConversions::PyToHwType(typeStr);
			m_Attributes.emplace_back(PythonAttribute(attribute.first, m_PyObject, type));
		}
	}

	void PythonEntity::DrawGui()
	{
		for (auto& attribute : m_Attributes)
		{
			attribute.DrawGui();
		}
	}

	void PythonEntity::Serialize(Serializer& serializer)
	{
		serializer.Add(m_PyClassName, "m_PyClassName");
		for (auto& attribute : m_Attributes)
		{
			serializer.Add<HollowareObject*>(&attribute, attribute.GetName().c_str());
		}
	}

	void PythonEntity::Deserialize(const Serializer& serializer)
	{
		serializer.Deserialize(m_PyClassName, "m_PyClassName");
		for (auto& attribute : m_Attributes)
		{
			serializer.Deserialize<HollowareObject>(attribute, attribute.GetName());
		}
	}
}