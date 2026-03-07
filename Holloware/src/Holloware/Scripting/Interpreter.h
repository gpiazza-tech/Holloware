#pragma once

struct TCCState;

namespace Holloware
{
	class Entity;
	struct ScriptProperty;

	class Interpreter
	{
	public:
		static void Begin();
		static void End();

		static TCCState* GenerateScriptState(const std::string& src);
		static std::vector<ScriptProperty> GenerateProperties(const std::string& src);
	};
}