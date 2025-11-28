#pragma once

#include "Holloware/Core/Timestep.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

namespace Holloware
{
	class PythonEntity
	{
	public:
		PythonEntity() = default;
		PythonEntity(pybind11::object pythonObject, Entity entity) 
			: m_PythonObject(pythonObject) 
		{
			m_PythonObject.attr("transform") = py::cast(&entity.GetComponent<TransformComponent>(), py::return_value_policy::reference);

			m_PythonObject.attr("position") = py::cast(&entity.GetComponent<TransformComponent>().Position, py::return_value_policy::reference);
			m_PythonObject.attr("rotation") = py::cast(&entity.GetComponent<TransformComponent>().Rotation, py::return_value_policy::reference);
			m_PythonObject.attr("scale") = py::cast(&entity.GetComponent<TransformComponent>().Scale, py::return_value_policy::reference);

		};
		~PythonEntity() {}

		void OnStart() { m_PythonObject.attr("on_start")(); }
		void OnUpdate(Timestep ts) { m_PythonObject.attr("on_update")(); }
		void OnDestroy() { m_PythonObject.attr("on_destroy")(); }
	private:
		pybind11::object m_PythonObject;
	};
}