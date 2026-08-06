#pragma once

#include <Perplex/Core/FileIO.h>

#include <nlohmann/json.hpp>

#include <fstream>
#include <string>
#include <filesystem>

#define PERPLEX_JSON_TO(v1) nlohmann_json_j[#v1] = nlohmann_json_t.v1;
#define PERPLEX_JSON_FROM(v1) nlohmann_json_t.v1 = nlohmann_json_j.value(#v1, nlohmann_json_t.v1);

#define PERPLEX_DEFINE_JSON_STRUCT(Type, ...)  \
    template<typename BasicJsonType, nlohmann::detail::enable_if_t<nlohmann::detail::is_basic_json<BasicJsonType>::value, int> = 0> \
    void to_json(BasicJsonType& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(PERPLEX_JSON_TO, __VA_ARGS__)) } \
    template<typename BasicJsonType, nlohmann::detail::enable_if_t<nlohmann::detail::is_basic_json<BasicJsonType>::value, int> = 0> \
    void from_json(const BasicJsonType& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(PERPLEX_JSON_FROM, __VA_ARGS__)) }

namespace Perplex
{
	const static std::filesystem::path s_QuickSavePath{ "save.data" };

	class JsonHelper
	{
	public:
		static void WriteToFile(const nlohmann::json& j, const std::filesystem::path& path) noexcept;
		static nlohmann::json LoadFromFile(const std::filesystem::path& path) noexcept;

		template<typename T>
		static void ObjectToFile(const T& obj, const std::filesystem::path& path) noexcept
		{
			nlohmann::json json{};

			std::filesystem::path pathStem = path.stem();
			json[pathStem.string().c_str()] = obj;

			std::error_code errorCode{};

			if (!path.parent_path().empty())
				std::filesystem::create_directories(path.parent_path(), errorCode);

			if (errorCode)
				HW_CORE_ERROR("Error creating directories for filepath {0}", path.string().c_str());

			std::ofstream output(path.string());
			output << json.dump(1);
			output.close();
		}

		template<typename T>
		static void ObjectFromFile(T& obj, const std::filesystem::path& path) noexcept
		{
			std::ifstream jsonFile(path.string());
			if (!jsonFile)
			{
				HW_CORE_INFO("file {0} does not exist, obj will be left unchanged.", path.string());
				return;
			}

			nlohmann::json json = nlohmann::json::parse(jsonFile);
			jsonFile.close();

			std::filesystem::path pathStem = path.stem();
			obj = json.value<T>(pathStem.string().c_str(), obj);
		}

		template<typename T>
		static void SaveValue(const char* key, const T& value) noexcept
		{
			const std::filesystem::path savePath{ FileIO::GameRootDirectory() / s_QuickSavePath };

			nlohmann::json json = LoadFromFile(savePath);
			json[key] = value;
			WriteToFile(json, savePath);
		}

		template<typename T>
		static void LoadValue(const char* key, T& value) noexcept
		{
			const std::filesystem::path savePath{ FileIO::GameRootDirectory() / s_QuickSavePath };

			nlohmann::json json = LoadFromFile(savePath);
			if (json.contains(key))
				value = json.value<T>(key, value);
		}
	};
}