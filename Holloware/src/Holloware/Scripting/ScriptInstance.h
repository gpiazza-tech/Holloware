#pragma once

struct TCCState;

namespace Holloware
{
	class ScriptInstance
	{
	public:
		ScriptInstance();
		~ScriptInstance();

		void BindTransform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
		bool Compile(const std::string& src);
		void BindFunctions();

		void TryCallStart() { if (m_Start != nullptr) m_Start(); }
		void TryCallUpdate(float ts) { if (m_Update != nullptr) m_Update(ts); }
		void TryCallStop() { if (m_Stop != nullptr) m_Stop(); }
	private:
		int (*m_Start)() = nullptr;
		int (*m_Update)(float ts) = nullptr;
		int (*m_Stop)() = nullptr;

		TCCState* m_State;
	};
}