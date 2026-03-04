#pragma once

namespace Holloware
{
	struct PythonScriptComponent;
	class Entity;

	static class PythonInterpreter
	{
	public:
		static void Begin();
		static void End();

		static void BindEntityToScript(Entity entity, PythonScriptComponent& psc);
		static void FreeEntityFromScript(Entity entity, PythonScriptComponent& psc);
	private:
		static void ExecutePyFile(const std::filesystem::path& path);
		static void ExecutePyFilesAt(const std::filesystem::path& path);
	};
}