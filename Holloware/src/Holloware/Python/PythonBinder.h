#pragma once

#include "Holloware/Scene/Components.h"
#include "Holloware/Scene/Entity.h"

namespace Holloware
{
	class PythonBinder
	{
	public:
		PythonBinder() = default;
		~PythonBinder() {};

		void BeginInterpreter();
		void EndInterpreter();

		void BindPythonScriptComponentFunctions(PythonScriptComponent& psc, Entity entity);

		void ExecutePyFile(std::string path);
		void ExecutePyFilesAt(std::string path);
	};
}