#pragma once

struct TCCState;

namespace Holloware
{
	class Entity;

	struct ScriptProperty
	{
		std::string Name;
		std::string Type;
		std::any Value;
	};

	struct ScriptContext
	{
		glm::vec3 PositionBuf;
		glm::vec3 RotationBuf;
		glm::vec3 ScaleBuf;
	};

	class ScriptData
	{
	public:
		ScriptData() = default;
		ScriptData(const std::string& source);
		~ScriptData();

		void TryCallStart(Entity context);
		void TryCallUpdate(float ts, Entity context);
		void TryCallStop(Entity context);

		const std::string& GetSource() { return m_Source; }
		const std::vector<ScriptProperty>& GetProperties() { return m_Properties; }
	private:
		bool Compile();
	private:
		TCCState* m_State;
		std::string m_Source;
		std::vector<ScriptProperty> m_Properties;

		ScriptContext m_Context;

		int (*m_Start)();
		int (*m_Update)(float ts);
		int (*m_Stop)();
	};
}