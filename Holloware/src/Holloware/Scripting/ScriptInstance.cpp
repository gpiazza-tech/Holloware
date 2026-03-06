#include <hwpch.h>
#include "ScriptInstance.h"

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Project.h"

#include <libtcc.h>

namespace Holloware
{
	void TCCErrorHandler(void* opaque, const char* msg)
	{
		HW_CORE_ERROR("C Script Error: {0}", msg);
	}

	ScriptInstance::ScriptInstance()
	{
		m_Start = nullptr;
		m_Update = nullptr;
		m_Stop = nullptr;

		m_State = tcc_new();
		tcc_set_error_func(m_State, nullptr, TCCErrorHandler);

		tcc_set_lib_path(m_State, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\lib");
		tcc_add_library_path(m_State, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\win32\\lib");

		tcc_add_include_path(m_State, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\include");
		tcc_add_include_path(m_State, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\win32\\include");
		std::string projectIncludePath = (Application::Get().GetCurrentProject().GetProjectPath() / "engine/tcc/include").string();
		tcc_add_include_path(m_State, projectIncludePath.c_str());

		tcc_set_output_type(m_State, TCC_OUTPUT_MEMORY);

		tcc_define_symbol(m_State, "PROPERTY", "");
		tcc_define_symbol(m_State, "EXTERN", "__declspec(dllimport)");
	}

	ScriptInstance::~ScriptInstance()
	{
		tcc_delete(m_State);
		m_Start = nullptr;
		m_Update = nullptr;
		m_Stop = nullptr;
	}

	void ScriptInstance::BindTransform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
	{
		tcc_add_symbol(m_State, "position", &position);
		tcc_add_symbol(m_State, "rotation", &rotation);
		tcc_add_symbol(m_State, "scale", &scale);
	}

	bool ScriptInstance::Compile(const std::string& src)
	{
		int output = tcc_compile_string(m_State, src.c_str());
		if (output == -1) return false;
		output = tcc_relocate(m_State);
		if (output == -1) return false;
		return true;
	}

	void ScriptInstance::BindFunctions()
	{
		m_Start = reinterpret_cast<int(*)()>(tcc_get_symbol(m_State, "start"));
		m_Update = reinterpret_cast<int(*)(float ts)>(tcc_get_symbol(m_State, "update"));
		m_Stop = reinterpret_cast<int(*)()>(tcc_get_symbol(m_State, "update"));
	}
}