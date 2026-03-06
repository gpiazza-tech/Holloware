#include <hwpch.h>
#include "Interpreter.h"

#include "Holloware/Core/Application.h"
#include "Holloware/Core/Project.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Scene/Components.h"
#include "Holloware/Scripting/ScriptProperty.h"
#include "Holloware/Scripting/ScriptInstance.h"

namespace Holloware
{
	namespace fs = std::filesystem;

	std::vector<ScriptInstance*> s_Instances = std::vector<ScriptInstance*>();

	void Interpreter::Begin()
	{
	}

	void Interpreter::End()
	{
		for (auto& instance : s_Instances)
			delete instance;
		 
		s_Instances.clear();
	}

	std::vector<ScriptProperty> Interpreter::GetProperties(const std::string& src)
	{
		return std::vector<ScriptProperty>();
	}

	void Interpreter::BindEntityToScript(const std::string& src, Entity entity)
	{
		ScriptComponent& sc = entity.GetComponent<ScriptComponent>();

		sc.Instance = new ScriptInstance();
		s_Instances.push_back(sc.Instance);

		TransformComponent& tc = entity.GetComponent<TransformComponent>();

		sc.Instance->BindTransform(tc.Position, tc.Rotation, tc.Scale);

		std::ifstream ifs(sc.ScriptAsset.GetPath().string());
		std::ostringstream oss;
		oss << ifs.rdbuf();

		if (sc.Instance->Compile(oss.str()))
		{
			sc.Instance->BindFunctions();
		}
		else 
		{
			delete sc.Instance;
			sc.Instance = nullptr;
			s_Instances.pop_back();
		}

		// TODO: Set each script property value, e.g.
		// for (auto& property : entity.GetComponent<ScriptComponent>().Properties)
		// {
		//		*(reinterpret_cast<int*>(tcc_get_symbol(s, property.Name))) = property.Value;
		// }
	}
}