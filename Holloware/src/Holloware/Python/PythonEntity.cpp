#include <hwpch.h>
#include "PythonEntity.h"

#include "Holloware/Python/PythonAttribute.h"
#include "Holloware/Scene/Components.h"
#include "Holloware/Scene/Entity.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace py = pybind11;

namespace Holloware
{
	PythonEntity::PythonEntity(const std::string& pyClassName, Entity entity)
		: m_PyClassName(pyClassName), m_PyObject(py::object(py::globals()[pyClassName.c_str()]()))
	{
		m_PyObject.attr("transform") = py::cast(&entity.GetComponent<TransformComponent>(), py::return_value_policy::reference);

		m_PyObject.attr("position") = py::cast(&entity.GetComponent<TransformComponent>().Position, py::return_value_policy::reference);
		m_PyObject.attr("rotation") = py::cast(&entity.GetComponent<TransformComponent>().Rotation, py::return_value_policy::reference);
		m_PyObject.attr("scale") = py::cast(&entity.GetComponent<TransformComponent>().Scale, py::return_value_policy::reference);
	}

	PythonEntity::~PythonEntity()
	{
	}

	void PythonEntity::OnStart() { m_PyObject.attr("on_start")(); }
	void PythonEntity::OnUpdate(Timestep ts) { m_PyObject.attr("on_update")(ts); }
	void PythonEntity::OnDestroy() { m_PyObject.attr("on_destroy")(); }

	void PythonEntity::UpdateAttributes()
	{
		py::dict pyAttributes = m_PyObject.attr("__dict__");

		m_Attributes = std::vector<PythonAttribute>();
		m_Attributes.reserve(py::len(pyAttributes));

		for (auto& attribute : pyAttributes)
		{
			// Name
			std::string objName = attribute.first.cast<std::string>();

			bool privateAttr = objName.c_str()[0] == '_' || objName == "position" || objName == "rotation" || objName == "scale" || objName == "transform";
			if (privateAttr) { continue; }

			// Value
			auto& typeObj = attribute.second.get_type();
			py::str typeObjPyStr = typeObj.attr("__name__");
			std::string typeStr = typeObjPyStr.cast<std::string>();

			m_Attributes.emplace_back(PythonAttribute(objName.c_str(), typeStr.c_str(), m_PyObject, attribute.first));
		}
	}

	std::vector<PythonAttribute> PythonEntity::GetAttributes()
	{
		return m_Attributes;
	}
}