#include <hwpch.h>
#include "PythonBinder.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

namespace Holloware
{
	PythonBinder::PythonBinder()
	{
		/*py::initialize_interpreter();

		py::exec(R"(
			import example

			a = example.add(1, 2)
		)");*/
	}

	PythonBinder::~PythonBinder()
	{
		// py::finalize_interpreter();
	}

	void PythonBinder::Test()
	{
		py::scoped_interpreter guard{};

		try
		{
			py::exec(R"(
import sys
import math
import holloware

a = holloware.add(4, 2)
holloware.log(str(a) + " it really worksssss!")
			)");
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("Embedded python syntax error!");
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

	PYBIND11_EMBEDDED_MODULE(holloware, m, py::mod_gil_not_used())
	{
		m.doc() = "pybind11 example plugin";

		m.def("add", &add, "A function that adds two numbers");
		m.def("log", &log, "A function that prints something");
	}
}