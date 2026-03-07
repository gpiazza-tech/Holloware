#include <hwpch.h>
#include "ScriptData.h"

#include "Holloware/Scripting/Interpreter.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Scene/Components.h"

#include <libtcc.h>

namespace Holloware
{
	ScriptData::ScriptData(const std::string& source)
		: m_Source(source)
	{
		m_Properties = Interpreter::GenerateProperties(m_Source);
		m_State = Interpreter::GenerateScriptState(m_Source);

		glm::vec3* posPtr = new glm::vec3();
		glm::vec3* rotPtr = new glm::vec3();
		glm::vec3* sclPtr = new glm::vec3();

		/*tcc_add_symbol(m_State, "position", posPtr);
		tcc_add_symbol(m_State, "rotation", rotPtr);
		tcc_add_symbol(m_State, "scale", sclPtr);*/

		tcc_add_symbol(m_State, "position", &m_Context.PositionBuf);
		tcc_add_symbol(m_State, "rotation", &m_Context.RotationBuf);
		tcc_add_symbol(m_State, "scale", &m_Context.ScaleBuf);

		bool compileSuccessful = Compile();

		if (compileSuccessful)
		{
			m_Start = reinterpret_cast<int(*)()>(tcc_get_symbol(m_State, "start"));
			m_Update = reinterpret_cast<int(*)(float ts)>(tcc_get_symbol(m_State, "update"));
			m_Stop = reinterpret_cast<int(*)()>(tcc_get_symbol(m_State, "update"));
		}
	}

	ScriptData::~ScriptData()
	{
		tcc_delete(m_State);
	}

	void ScriptData::TryCallStart(Entity context)
	{
		if (m_Start != nullptr)
		{
			TransformComponent& contextTransform = context.GetComponent<TransformComponent>();

			m_Context.PositionBuf = contextTransform.Position;
			m_Context.RotationBuf = contextTransform.Rotation;
			m_Context.ScaleBuf = contextTransform.Scale;

			m_Start();

			contextTransform.Position = m_Context.PositionBuf;
			contextTransform.Rotation = m_Context.RotationBuf;
			contextTransform.Scale = m_Context.ScaleBuf;
		}
	}

	void ScriptData::TryCallUpdate(float ts, Entity context)
	{
		if (m_Update != nullptr)
		{
			TransformComponent& contextTransform = context.GetComponent<TransformComponent>();

			m_Context.PositionBuf = contextTransform.Position;
			m_Context.RotationBuf = contextTransform.Rotation;
			m_Context.ScaleBuf = contextTransform.Scale;

			m_Update(ts);

			contextTransform.Position = m_Context.PositionBuf;
			contextTransform.Rotation = m_Context.RotationBuf;
			contextTransform.Scale = m_Context.ScaleBuf;
		}
	}

	void ScriptData::TryCallStop(Entity context)
	{
		if (m_Stop != nullptr)
		{
			TransformComponent& contextTransform = context.GetComponent<TransformComponent>();

			m_Context.PositionBuf = contextTransform.Position;
			m_Context.RotationBuf = contextTransform.Rotation;
			m_Context.ScaleBuf = contextTransform.Scale;

			m_Stop();

			contextTransform.Position = m_Context.PositionBuf;
			contextTransform.Rotation = m_Context.RotationBuf;
			contextTransform.Scale = m_Context.ScaleBuf;
		}
	}

	bool ScriptData::Compile()
	{
		int output = tcc_compile_string(m_State, m_Source.c_str());
		if (output == -1) return false;
		output = tcc_relocate(m_State);
		if (output == -1) return false;
		return true;
	}
}