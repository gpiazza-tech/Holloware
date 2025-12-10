#pragma once

#include <pybind11/pybind11.h>

namespace Holloware
{
	class Entity;
	class Timestep;
	class PythonAttribute;

	class PythonEntity
	{
	public:
		PythonEntity() = default;
		PythonEntity(const std::string& pyClassName, Entity entity);
		~PythonEntity();

		void OnStart();
		void OnUpdate(Timestep ts);
		void OnDestroy();

		void UpdateAttributes();
		std::vector<PythonAttribute> GetAttributes();
	private:
		pybind11::object m_PyObject;
		std::vector<PythonAttribute> m_Attributes;
		std::string m_PyClassName;
	};
};