#include <hwpch.h>
#include "PythonBinder.h"

#include "Holloware/Python/PythonEntity.h"

#include <iostream>
#include <fstream>
#include <Holloware/Scene/Entity.h>

namespace py = pybind11;

namespace Holloware
{
	void PythonBinder::BeginInterpreter()
	{
		py::initialize_interpreter();

		ExecutePyFile("assets/scripts/ScriptableObject.py");
	}

	void PythonBinder::EndInterpreter()
	{
		py::finalize_interpreter();
	}

	void PythonBinder::BindPythonScriptComponentFunctions(PythonScriptComponent& psc)
	{
		auto mainModule = py::module_::import("__main__");
		auto globals = py::globals();

		try
		{
			ExecutePyFile(psc.Filepath.string());

			std::string className = psc.Filepath.stem().string();

			const py::object& pyClass = globals[className.c_str()];
			py::object pyClassInstance = pyClass();

			psc.Instance = new PythonEntity(pyClassInstance);
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("Embedded python syntax error in {0}", psc.Filepath.string());
		}
	}

	void PythonBinder::ExecutePyFile(std::string path)
	{
		std::ifstream ifs(path);
		std::ostringstream oss;
		oss << ifs.rdbuf();

		const std::string& pyCode = oss.str();

		try
		{
			py::exec(pyCode);
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("Python syntax error in {0}", path);
		}
	}

	int add(int i, int j)
	{
		return i + j;
	}

	void log(const char* log)
	{
		HW_CORE_TRACE(log);
	}

	PYBIND11_EMBEDDED_MODULE(hw, m, py::mod_gil_not_used())
	{
		m.doc() = "pybind11 example plugin";

		m.def("add", &add, "A function that adds two numbers");
		m.def("log", &log, "A function that prints something");

		py::class_<TransformComponent>(m, "TransformComponent")
			.def_readwrite("translation", &TransformComponent::Translation)
			.def_readwrite("rotation", &TransformComponent::Rotation)
			.def_readwrite("scale", &TransformComponent::Scale);

		py::class_<Entity>(m, "Entity")
			.def("get_transform", &Entity::GetComponent<TransformComponent>);
	}
}