#pragma once

struct TCCState;

namespace Holloware
{
	class Entity;
	class ScriptProperty;

	class ScriptInstance
	{
	public:
		ScriptInstance() : m_State(nullptr) { }
		~ScriptInstance();

		bool IsCompiled() { return m_State != nullptr; }
		bool Compile(const std::string& src, Entity entity);

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