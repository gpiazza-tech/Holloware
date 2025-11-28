#include <hwpch.h>
#include "PythonBinder.h"

#include "Holloware/Python/PythonEntity.h"

#include <iostream>
#include <fstream>

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

	void PythonBinder::BindPythonScriptComponentFunctions(PythonScriptComponent& psc, Entity entity)
	{
		auto mainModule = py::module_::import("__main__");
		auto globals = py::globals();

		try
		{
			ExecutePyFile(psc.Filepath.string());

			std::string className = psc.Filepath.stem().string();

			const py::object& pyClass = globals[className.c_str()];
			py::object pyClassInstance = pyClass();

			psc.Instance = new PythonEntity(pyClassInstance, entity);
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

		py::class_<glm::vec3>(m, "Vec3")
			.def(py::init<>())
			.def(py::init<float, float, float>())
			.def_readwrite("x", &glm::vec3::x)
			.def_readwrite("y", &glm::vec3::y)
			.def_readwrite("z", &glm::vec3::z);

		py::class_<TransformComponent>(m, "TransformComponent")
			.def(py::init<>())
			.def_readwrite("position", &TransformComponent::Position)
			.def_readwrite("rotation", &TransformComponent::Rotation)
			.def_readwrite("scale", &TransformComponent::Scale);

		py::class_<Entity>(m, "Entity")
			.def(py::init<>())
			.def("transform", &Entity::GetComponent<TransformComponent>)
			.def_property_readonly("id", &Entity::GetUUID);

		py::class_<Timestep>(m, "Timestep")
			.def(py::init<float>())
			.def_property_readonly("get_seconds", &Timestep::GetSeconds)
			.def_property_readonly("get_milliseconds", &Timestep::GetMilliseconds);
	}
}