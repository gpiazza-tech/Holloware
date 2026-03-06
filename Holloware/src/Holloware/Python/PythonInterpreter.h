#pragma once

namespace Holloware
{
	struct Attribute;
	struct PythonScriptComponent;
	class Entity;

	static class PythonInterpreter
	{
	public:
		static void Begin();
		static void End();

		static void BindEntityToScript(Entity entity, PythonScriptComponent& psc);
		static void FreeEntityFromScript(Entity entity, PythonScriptComponent& psc);
		static std::vector<Attribute> GetAttributes(const std::string& src);
	private:
		static void ExecutePyFile(const std::filesystem::path& path);
		static void ExecutePyFilesAt(const std::filesystem::path& path);
	};
}