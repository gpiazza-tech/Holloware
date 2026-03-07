#include <hwpch.h>
#include "Interpreter.h"

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Project.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Scene/Components.h"
#include "Holloware/Scripting/ScriptInstance.h"
#include "Holloware/Scripting/ScriptData.h"

#include <libtcc.h>

namespace Holloware
{
	namespace fs = std::filesystem;

	void Interpreter::Begin()
	{
	}

	void Interpreter::End()
	{
	}

	TCCState* Interpreter::GenerateScriptState(const std::string& src)
	{
		TCCState* state = tcc_new();
		tcc_set_error_func(state, nullptr, [](void* opaque, const char* msg) { }); // Ignoring errors for now because we dont care for this compilation

		tcc_set_lib_path(state, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\lib");
		tcc_add_library_path(state, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\win32\\lib");

		tcc_add_include_path(state, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\include");
		tcc_add_include_path(state, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\win32\\include");
		std::string projectIncludePath = (Application::Get().GetCurrentProject().GetProjectPath() / "engine/tcc/include").string();
		tcc_add_include_path(state, projectIncludePath.c_str());

		tcc_set_output_type(state, TCC_OUTPUT_MEMORY);

		tcc_define_symbol(state, "PROPERTY", "");
		tcc_define_symbol(state, "EXTERN", "__declspec(dllimport)");

		return state;
	}

	std::vector<ScriptProperty> Interpreter::GenerateProperties(const std::string& src)
	{
		return std::vector<ScriptProperty>();
	}
}