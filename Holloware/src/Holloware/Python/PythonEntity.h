#pragma once

#include "Holloware/Core/Timestep.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace Holloware
{
	class PythonEntity
	{
	public:
		PythonEntity() = default;
		PythonEntity(pybind11::object instance) : m_Instance(instance) {};
		~PythonEntity() {}

		void OnStart() { m_Instance.attr("on_start")(); }
		void OnUpdate(Timestep ts) { m_Instance.attr("on_update")(); }
		void OnDestroy() { m_Instance.attr("on_destroy")(); }
	private:
		pybind11::object m_Instance;
	};
}