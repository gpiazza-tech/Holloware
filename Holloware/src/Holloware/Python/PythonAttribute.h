#pragma once

#include <pybind11/pybind11.h>

namespace Holloware
{
	class PythonAttribute
	{
	public:
		PythonAttribute() = default;
		PythonAttribute(const std::string& name, const std::string& type, const pybind11::object& object, pybind11::handle handle)
			: m_Name(name), m_Type(type), m_Handle(handle), m_Dict(object.attr("__dict__"))
		{
		}
		~PythonAttribute() {}

		inline const std::string& GetName() { return m_Name; }
		inline const std::string& GetType() { return m_Type; }

		template<typename T>
		inline T GetValue()
		{
			return m_Dict[m_Handle].cast<T>();
		}

		template<typename T>
		inline void SetValue(T val)
		{
			m_Dict[m_Handle] = val;
		}
		
	private:
		std::string m_Name;
		std::string m_Type;
		pybind11::handle m_Handle;

		pybind11::dict m_Dict;
	};
}