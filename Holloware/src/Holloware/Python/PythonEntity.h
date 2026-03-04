#pragma once

#include "Holloware/Core/HollowareObject.h"
 
#include <pybind11/pybind11.h>

namespace Holloware
{
	class Timestep;
	class PythonAttribute;
	class Serializer;

	class PythonEntity : public HollowareObject
	{
	public:
		PythonEntity() = default;
		PythonEntity(const std::string& pyClassName, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
		~PythonEntity();

		void OnStart();
		void OnUpdate(Timestep ts);
		void OnDestroy();

		void DrawGui() override;

		void Serialize(Serializer& serializer) override;
		void Deserialize(const Serializer& serializer) override;
	private:
		void BindTransform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
		void BindAttributes();
	private:
		pybind11::object m_PyObject;
		std::vector<PythonAttribute> m_Attributes;
		std::string m_PyClassName;
	};
};