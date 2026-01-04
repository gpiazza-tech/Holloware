#pragma once

#include "Holloware/Core/HollowareObject.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Holloware
{
	class Serializer
	{
	public:
		static Serializer LoadFromFile(const std::filesystem::path& path);
	public:
		Serializer() : m_Json(json()) {};
		Serializer(json j) : m_Json(j) {};

		template<typename T>
		void Add(T val, const char* name)
		{
			m_Json[name] = val;
		}

		template<>
		void Add<HollowareObject*>(HollowareObject* val, const char* name)
		{
			Serializer serializer(m_Json[name]);
			val->Serialize(serializer);
			m_Json[name] = serializer.GetJson();
		}

		template<typename T>
		void Deserialize(T& val, const char* name) const
		{
			val = m_Json[name].get<T>();
		}

		template<typename T>
		void Deserialize(T& val, const json& j) const
		{
			val = j.get<T>();
		}

		template<>
		void Deserialize<HollowareObject>(HollowareObject& val, const char* name) const
		{
			val.Deserialize(Serializer(m_Json[name]));
		}

		template<>
		void Deserialize<HollowareObject>(HollowareObject& val, const json& j) const
		{
			val.Deserialize(Serializer(j));
		}

		template<typename T>
		T GetValue(const std::string& key) const { return m_Json[key].get<T>(); }

		const json& GetJson() const { return m_Json; }
		const bool& Contains(const std::string& str) const { return m_Json.contains(str); }

		void WriteToFile(const std::filesystem::path& path);

	private:
		json m_Json;
	};
}

namespace glm
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec3, x, y, z)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(glm::vec4, r, g, b, a)
}