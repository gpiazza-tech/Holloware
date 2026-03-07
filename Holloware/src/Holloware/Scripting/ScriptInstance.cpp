#include <hwpch.h>
#include "ScriptInstance.h"

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Project.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Scene/Components.h"

#include <libtcc.h>

namespace Holloware
{
	ScriptInstance::~ScriptInstance()
	{
		if (m_State != nullptr)
			tcc_delete(m_State);
	}

	glm::vec3* get_position(Entity e) { return &e.GetComponent<TransformComponent>().Position; }
	glm::vec3* get_rotation(Entity e) { return &e.GetComponent<TransformComponent>().Rotation; }
	glm::vec3* get_scale(Entity e) { return &e.GetComponent<TransformComponent>().Scale; }
	void console_trace(const char* msg) { HW_TRACE(msg); }
	void console_info(const char* msg) { HW_INFO(msg); }
	void console_warn(const char* msg) { HW_WARN(msg); }
	void console_error(const char* msg) { HW_ERROR(msg); }

	bool ScriptInstance::Compile(const std::string& src, Entity entity)
	{
		if (m_State != nullptr)
		{
			tcc_delete(m_State);
			m_State = nullptr;
		}
		m_State = tcc_new();

		// Setup the script's environment.
		tcc_set_error_func(m_State, nullptr, [](void* opaque, const char* msg) { HW_CORE_ERROR("C Script Error: {0}", msg); });

		tcc_set_lib_path(m_State, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\lib");
		tcc_add_library_path(m_State, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\win32\\lib");

		tcc_add_include_path(m_State, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\include");
		tcc_add_include_path(m_State, "C:\\dev\\Holloware\\Holloware\\vendor\\tcc\\win32\\include");
		std::string projectIncludePath = (Application::Get().GetCurrentProject().GetProjectPath() / "engine/tcc/include").string();
		tcc_add_include_path(m_State, projectIncludePath.c_str());

		tcc_set_output_type(m_State, TCC_OUTPUT_MEMORY);

		tcc_define_symbol(m_State, "PROPERTY", "");
		tcc_define_symbol(m_State, "EXTERN", "__declspec(dllimport)");

		// Bind transform
		tcc_add_symbol(m_State, "entity", &entity);
		tcc_add_symbol(m_State, "position", &entity.GetComponent<TransformComponent>().Position);
		tcc_add_symbol(m_State, "rotation", &entity.GetComponent<TransformComponent>().Rotation);
		tcc_add_symbol(m_State, "scale", &entity.GetComponent<TransformComponent>().Scale);

		// Bind host functions
		tcc_add_symbol(m_State, "get_position", get_position);
		tcc_add_symbol(m_State, "get_rotation", get_rotation);
		tcc_add_symbol(m_State, "get_scale", get_scale);

		tcc_add_symbol(m_State, "console_trace", console_trace);
		tcc_add_symbol(m_State, "console_info", console_info);
		tcc_add_symbol(m_State, "console_warn", console_warn);
		tcc_add_symbol(m_State, "console_error", console_error);

		// Compile
		if (tcc_compile_string(m_State, src.c_str()) == -1)
		{
			tcc_delete(m_State);
			m_State = nullptr;
			return false;
		}

		if (tcc_relocate(m_State) == -1)
		{
			tcc_delete(m_State);
			m_State = nullptr;
			return false;
		}

		// Bind callbacks
		m_Start = reinterpret_cast<int(*)()>(tcc_get_symbol(m_State, "start"));
		m_Update = reinterpret_cast<int(*)(float ts)>(tcc_get_symbol(m_State, "update"));
		m_Stop = reinterpret_cast<int(*)()>(tcc_get_symbol(m_State, "stop"));

		return true;
	}
}