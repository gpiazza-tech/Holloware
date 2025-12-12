#pragma once

namespace Holloware
{
	class SerializableValue
	{
	public:
		template<typename T>
		SerializableValue(T data, const char* name)
		{
			m_Data = (void*)(&data);
			m_Size = sizeof(T);
			m_Name = name;
		}
		~SerializableValue() {}

		template<typename T>
		const T& GetData()
		{
			return *(T*)m_Data;
		}
	private:
		void* m_Data;
		size_t m_Size;
		const char* m_Name;
	};
}