#pragma once

namespace Holloware
{
	struct PythonScriptComponent;
	class Entity;

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

		inline bool Interpreting() const { return m_Interpreting; }
	private:
		bool m_Interpreting = false; 
	};
}