#pragma once

#include "Holloware/Scene/Components.h"

namespace Holloware
{
	class PythonBinder
	{
	public:
		PythonBinder() = default;
		~PythonBinder() {};

		void BeginInterpreter();
		void EndInterpreter();

		void BindPythonScriptComponentFunctions(PythonScriptComponent& psc);

		void ExecutePyFile(std::string path);
	};
}