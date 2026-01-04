#include <hwpch.h>
#include "PythonBinder.h"

#include "Holloware/Scene/Entity.h"
#include "Holloware/Scene/Components.h"
#include "Holloware/Python/PythonEntity.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

namespace Holloware
{
	void PythonBinder::BeginInterpreter()
	{
		HW_PROFILE_FUNCTION();

		py::initialize_interpreter();
		m_Interpreting = true;

		ExecutePyFile("assets/scripts/ScriptableObject.py");
	}

	void PythonBinder::EndInterpreter()
	{
		HW_PROFILE_FUNCTION();

		py::finalize_interpreter();
		m_Interpreting = false;
	}

	void PythonBinder::BindPythonScriptComponentFunctions(PythonScriptComponent& psc, Entity entity)
	{
		HW_PROFILE_FUNCTION();

		auto mainModule = py::module_::import("__main__");
		auto globals = py::globals();

		try
		{
			std::string className = std::filesystem::path(psc.Filepath).stem().string();

			// Creating class instance in python on the heap
			psc.Instance = new PythonEntity(className.c_str(), entity);
			psc.Instance->UpdateAttributes();
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("Embedded python syntax error in {0}", psc.Filepath);
		}
	}

	void PythonBinder::ExecutePyFile(const std::filesystem::path& path)
	{
		HW_PROFILE_FUNCTION();

		std::ifstream ifs(path.string());
		std::ostringstream oss;
		oss << ifs.rdbuf();

		const std::string& pyCode = oss.str();

		try
		{
			py::exec(pyCode);
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("Python syntax error in {0}", path.string());
		}
	}

	void PythonBinder::ExecutePyFilesAt(const std::filesystem::path& path)
	{
		HW_PROFILE_FUNCTION();

		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			if (file.is_regular_file() && file.path().extension() == ".py")
				ExecutePyFile(file.path().string());
		}

		std::ifstream ifs(path.string());
		std::ostringstream oss;
		oss << ifs.rdbuf();

		const std::string& pyCode = oss.str();

		try
		{
			py::exec(pyCode);
		}
		catch (std::exception e)
		{
			HW_CORE_ERROR("Python syntax error in {0}", path.string());
		}
	}

	PYBIND11_EMBEDDED_MODULE(hw, m, py::mod_gil_not_used())
	{
		m.doc() = "pybind11 example plugin";
		m.def("log", [](const char* log) { HW_CORE_TRACE(log); }, "A function that prints something");

		py::class_<glm::vec3>(m, "Vec3")
			.def(py::init<>())
			.def(py::init<float, float, float>())
			.def_readwrite("x", &glm::vec3::x)
			.def_readwrite("y", &glm::vec3::y)
			.def_readwrite("z", &glm::vec3::z);

		py::class_<TransformComponent>(m, "TransformComponent")
			.def(py::init<>())
			.def_readwrite("position", &TransformComponent::Position)
			.def_readwrite("rotation", &TransformComponent::Rotation)
			.def_readwrite("scale", &TransformComponent::Scale);

		py::class_<Entity>(m, "Entity")
			.def(py::init<>())
			.def_property("transform", &Entity::GetComponent<TransformComponent>, &Entity::GetComponent<TransformComponent>)
			.def_property("tag", &Entity::GetComponent<TagComponent>, &Entity::GetComponent<TagComponent>)
			.def_property_readonly("valid", &Entity::operator bool)
			.def_property_readonly("id", &Entity::GetUUID);

		py::class_<Timestep>(m, "Timestep")
			.def(py::init<float>())
			.def_property_readonly("get_seconds", &Timestep::GetSeconds)
			.def_property_readonly("get_milliseconds", &Timestep::GetMilliseconds);

		py::class_<char>(m, "Math")
			.def_static("clamp", glm::clamp<float>)
			.def_static("min", &glm::min<float>)
			.def_static("max", &glm::max<float>)
			.def_static("floor", [](float x) { return glm::floor(x); })
			.def_static("ceil", [](float x) { return glm::ceil(x); })
			.def_static("sin", [](float x) { return glm::sin(x); })
			.def_static("cos", [](float x) { return glm::cos(x); })
			.def_static("tan", [](float x) { return glm::tan(x); })
			.def_static("arcsin", [](float x) { return glm::asin(x); })
			.def_static("arccos", [](float x) { return glm::acos(x); })
			.def_static("arctan", [](float x) { return glm::atan(x); })
			.def_static("abs", &glm::abs<float>)
			.def_static("degrees", &glm::degrees<float>)
			.def_static("radians", &glm::radians<float>)
			.def_static("pi", &glm::pi<float>)
			.def_static("dot", &glm::dot<float>);
	}

	PYBIND11_EMBEDDED_MODULE(hw_input, m, py::mod_gil_not_used())
	{
		enum key : int {};

		py::enum_<key> keyEnum(m, "Key", "enum.IntEnum");
		keyEnum.value("SPACE", (key)HW_KEY_SPACE);
		keyEnum.value("APOSTROPHE", (key)HW_KEY_APOSTROPHE);
		keyEnum.value("COMMA", (key)HW_KEY_COMMA);
		keyEnum.value("MINUS", (key)HW_KEY_MINUS);
		keyEnum.value("PERIOD", (key)HW_KEY_PERIOD);
		keyEnum.value("SLASH", (key)HW_KEY_SLASH);
		keyEnum.value("0", (key)HW_KEY_0);
		keyEnum.value("1", (key)HW_KEY_1);
		keyEnum.value("2", (key)HW_KEY_2);
		keyEnum.value("3", (key)HW_KEY_3);
		keyEnum.value("4", (key)HW_KEY_4);
		keyEnum.value("5", (key)HW_KEY_5);
		keyEnum.value("6", (key)HW_KEY_6);
		keyEnum.value("7", (key)HW_KEY_7);
		keyEnum.value("8", (key)HW_KEY_8);
		keyEnum.value("9", (key)HW_KEY_9);
		keyEnum.value("SEMICOLON", (key)HW_KEY_SEMICOLON);
		keyEnum.value("EQUAL", (key)HW_KEY_EQUAL);
		keyEnum.value("A", (key)HW_KEY_A);
		keyEnum.value("B", (key)HW_KEY_B);
		keyEnum.value("C", (key)HW_KEY_C);
		keyEnum.value("D", (key)HW_KEY_D);
		keyEnum.value("E", (key)HW_KEY_E);
		keyEnum.value("F", (key)HW_KEY_F);
		keyEnum.value("G", (key)HW_KEY_G);
		keyEnum.value("H", (key)HW_KEY_H);
		keyEnum.value("I", (key)HW_KEY_I);
		keyEnum.value("J", (key)HW_KEY_J);
		keyEnum.value("K", (key)HW_KEY_K);
		keyEnum.value("L", (key)HW_KEY_L);
		keyEnum.value("M", (key)HW_KEY_M);
		keyEnum.value("N", (key)HW_KEY_N);
		keyEnum.value("O", (key)HW_KEY_O);
		keyEnum.value("P", (key)HW_KEY_P);
		keyEnum.value("Q", (key)HW_KEY_Q);
		keyEnum.value("R", (key)HW_KEY_R);
		keyEnum.value("S", (key)HW_KEY_S);
		keyEnum.value("T", (key)HW_KEY_T);
		keyEnum.value("U", (key)HW_KEY_U);
		keyEnum.value("V", (key)HW_KEY_V);
		keyEnum.value("W", (key)HW_KEY_W);
		keyEnum.value("X", (key)HW_KEY_X);
		keyEnum.value("Y", (key)HW_KEY_Y);
		keyEnum.value("Z", (key)HW_KEY_Z);
		keyEnum.value("LEFT_BRACKET", (key)HW_KEY_LEFT_BRACKET);
		keyEnum.value("BACKSLASH", (key)HW_KEY_BACKSLASH);
		keyEnum.value("RIGHT_BRACKET", (key)HW_KEY_RIGHT_BRACKET);
		keyEnum.value("GRAVE_ACCENT", (key)HW_KEY_GRAVE_ACCENT);
		keyEnum.value("WORLD_1", (key)HW_KEY_WORLD_1);
		keyEnum.value("WORLD_2", (key)HW_KEY_WORLD_2);

		// FUNCTION KEYS
		keyEnum.value("ESCAPE", (key)HW_KEY_ESCAPE);
		keyEnum.value("ENTER", (key)HW_KEY_ENTER);
		keyEnum.value("TAB", (key)HW_KEY_TAB);
		keyEnum.value("BACKSPACE", (key)HW_KEY_BACKSPACE);
		keyEnum.value("INSERT", (key)HW_KEY_INSERT);
		keyEnum.value("DELETE", (key)HW_KEY_DELETE);
		keyEnum.value("RIGHT", (key)HW_KEY_RIGHT);
		keyEnum.value("LEFT", (key)HW_KEY_LEFT);
		keyEnum.value("DOWN", (key)HW_KEY_DOWN);
		keyEnum.value("UP", (key)HW_KEY_UP);
		keyEnum.value("PAGE_UP", (key)HW_KEY_PAGE_UP);
		keyEnum.value("PAGE_DOWN", (key)HW_KEY_PAGE_DOWN);
		keyEnum.value("HOME", (key)HW_KEY_HOME);
		keyEnum.value("END", (key)HW_KEY_END);
		keyEnum.value("CAPS_LOCK", (key)HW_KEY_CAPS_LOCK);
		keyEnum.value("SCROLL_LOCK", (key)HW_KEY_SCROLL_LOCK);
		keyEnum.value("NUM_LOCK", (key)HW_KEY_NUM_LOCK);
		keyEnum.value("PRINT_SCREEN", (key)HW_KEY_PRINT_SCREEN);
		keyEnum.value("PAUSE", (key)HW_KEY_PAUSE);
		keyEnum.value("F1", (key)HW_KEY_F1);
		keyEnum.value("F2", (key)HW_KEY_F2);
		keyEnum.value("F3", (key)HW_KEY_F3);
		keyEnum.value("F4", (key)HW_KEY_F4);
		keyEnum.value("F5", (key)HW_KEY_F5);
		keyEnum.value("F6", (key)HW_KEY_F6);
		keyEnum.value("F7", (key)HW_KEY_F7);
		keyEnum.value("F8", (key)HW_KEY_F8);
		keyEnum.value("F9", (key)HW_KEY_F9);
		keyEnum.value("F10", (key)HW_KEY_F10);
		keyEnum.value("F11", (key)HW_KEY_F11);
		keyEnum.value("F12", (key)HW_KEY_F12);
		keyEnum.value("F13", (key)HW_KEY_F13);
		keyEnum.value("F14", (key)HW_KEY_F14);
		keyEnum.value("F15", (key)HW_KEY_F15);
		keyEnum.value("F16", (key)HW_KEY_F16);
		keyEnum.value("F17", (key)HW_KEY_F17);
		keyEnum.value("F18", (key)HW_KEY_F18);
		keyEnum.value("F19", (key)HW_KEY_F19);
		keyEnum.value("F20", (key)HW_KEY_F20);
		keyEnum.value("F21", (key)HW_KEY_F21);
		keyEnum.value("F22", (key)HW_KEY_F22);
		keyEnum.value("F23", (key)HW_KEY_F23);
		keyEnum.value("F24", (key)HW_KEY_F24);
		keyEnum.value("F25", (key)HW_KEY_F25);
		keyEnum.value("KP_0", (key)HW_KEY_KP_0);
		keyEnum.value("KP_1", (key)HW_KEY_KP_1);
		keyEnum.value("KP_2", (key)HW_KEY_KP_2);
		keyEnum.value("KP_3", (key)HW_KEY_KP_3);
		keyEnum.value("KP_4", (key)HW_KEY_KP_4);
		keyEnum.value("KP_5", (key)HW_KEY_KP_5);
		keyEnum.value("KP_6", (key)HW_KEY_KP_6);
		keyEnum.value("KP_7", (key)HW_KEY_KP_7);
		keyEnum.value("KP_8", (key)HW_KEY_KP_8);
		keyEnum.value("KP_9", (key)HW_KEY_KP_9);
		keyEnum.value("KP_DECIMAL", (key)HW_KEY_KP_DECIMAL);
		keyEnum.value("KP_DIVIDE", (key)HW_KEY_KP_DIVIDE);
		keyEnum.value("KP_MULTIPLY", (key)HW_KEY_KP_MULTIPLY);
		keyEnum.value("KP_SUBTRACT", (key)HW_KEY_KP_SUBTRACT);
		keyEnum.value("KP_ADD", (key)HW_KEY_KP_ADD);
		keyEnum.value("KP_ENTER", (key)HW_KEY_KP_ENTER);
		keyEnum.value("KP_EQUAL", (key)HW_KEY_KP_EQUAL);
		keyEnum.value("LEFT_SHIFT", (key)HW_KEY_LEFT_SHIFT);
		keyEnum.value("LEFT_CONTROL", (key)HW_KEY_LEFT_CONTROL);
		keyEnum.value("LEFT_ALT", (key)HW_KEY_LEFT_ALT);
		keyEnum.value("LEFT_SUPER", (key)HW_KEY_LEFT_SUPER);
		keyEnum.value("RIGHT_SHIFT", (key)HW_KEY_RIGHT_SHIFT);
		keyEnum.value("RIGHT_CONTROL", (key)HW_KEY_RIGHT_CONTROL);
		keyEnum.value("RIGHT_ALT", (key)HW_KEY_RIGHT_ALT);
		keyEnum.value("RIGHT_SUPER", (key)HW_KEY_RIGHT_SUPER);
		keyEnum.value("KEY_MENU", (key)HW_KEY_MENU);

		m.def("key_down", &Input::IsKeyPressed);
		m.def("mouse_button_down", &Input::IsMouseButtonPressed);
	}
}