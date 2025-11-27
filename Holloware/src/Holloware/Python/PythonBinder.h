#pragma once

namespace Holloware
{
	class PythonBinder
	{
	public:
		PythonBinder() = default;
		~PythonBinder() {};

		void BeginInterpreter();
		void EndInterpreter();

		void Test();
		void ExecutePyFile(std::string path);
	};
}