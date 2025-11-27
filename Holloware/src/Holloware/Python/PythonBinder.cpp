#include <hwpch.h>
#include "PythonBinder.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

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

	void PythonBinder::Test()
	{
		auto mainModule = py::module_::import("__main__");
		auto globals = py::globals();

		try
		{
			ExecutePyFile("assets/scripts/Player.py");

			py::object playerClass = globals["Player"];
			py::object playerObj = playerClass();

			playerObj.attr("on_start")();
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("Embedded python syntax error!");
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
	}
}