#pragma once

#include "Holloware/Core/HollowareObject.h"
#include "Holloware/Core/HollowareTypes.h"

#include <pybind11/pybind11.h>

namespace Holloware
{
	class PythonAttribute : HollowareObject
	{
	public:
		PythonAttribute() = default;
		PythonAttribute(const pybind11::handle& handle, const pybind11::object& object, HollowareTypes type)
			: m_Handle(handle), m_Object(object), m_Type(type)
		{
		}
		~PythonAttribute() {}

		inline std::string GetName() { return m_Handle.cast<std::string>(); }
		inline std::string GetType() { return pybind11::str(GetDict()[m_Handle].get_type()).cast<std::string>(); }

		template<typename T>
		inline T GetValue()
		{
			return GetDict()[m_Handle].cast<T>();
		}

		template<typename T>
		inline void SetValue(T val)
		{
			GetDict()[m_Handle] = val;
		}

		void DrawGui() override;
	private:
		inline const pybind11::dict& GetDict() { return m_Object.attr("__dict__"); }
		
	private:
		pybind11::handle m_Handle;
		pybind11::object m_Object;
		HollowareTypes m_Type;
	};
}