#include <hwpch.h>

#include "Holloware/Scene/Components.h"
#include "Holloware/Scene/Entity.h"
#include "Holloware/Core/Timestep.h"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <glm/glm.hpp>

namespace py = pybind11;

namespace Holloware 
{
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
			.def("transform", &Entity::GetComponent<TransformComponent>)
			.def_property_readonly("id", &Entity::GetUUID);

		py::class_<Timestep>(m, "Timestep")
			.def(py::init<float>())
			.def_property_readonly("get_seconds", &Timestep::GetSeconds)
			.def_property_readonly("get_milliseconds", &Timestep::GetMilliseconds);
	}

	PYBIND11_EMBEDDED_MODULE(hw_input, m, py::mod_gil_not_used())
	{
		enum key : int {};

		py::enum_<key>(m, "Key", "enum.IntEnum")
			.value("W", (key)HW_KEY_W)
			.value("A", (key)HW_KEY_A)
			.value("S", (key)HW_KEY_S)
			.value("D", (key)HW_KEY_D)
			.value("ESCAPE", (key)HW_KEY_ESCAPE);

		m.def("key_down", &Input::IsKeyPressed);
		m.def("mouse_button_down", &Input::IsMouseButtonPressed);
	}
}