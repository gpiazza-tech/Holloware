#pragma once

namespace Holloware
{
	class Entity;
	struct ScriptProperty;

	class Interpreter
	{
	public:
		static void Begin();
		static void End();

		static std::vector<ScriptProperty> GetProperties(const std::string& src);
		static void BindEntityToScript(const std::string& src, Entity entity);
	};
}