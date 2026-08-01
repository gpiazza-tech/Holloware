#pragma once

namespace Perplex
{
	// WILL BE HANDLED BY C++26 REFLECTIONS IN THE FUTURE

	class CUnit;

	struct IDComponent;
	void BindSymbols(CUnit& unit, IDComponent);

	struct TagComponent;
	void BindSymbols(CUnit& unit, TagComponent);

	struct EnableComponent;
	void BindSymbols(CUnit& unit, EnableComponent);

	struct TransformComponent;
	void BindSymbols(CUnit& unit, TransformComponent);

	struct SpriteRendererComponent;
	void BindSymbols(CUnit& unit, SpriteRendererComponent);

	struct BoxRendererComponent;
	void BindSymbols(CUnit& unit, BoxRendererComponent);

	struct CircleRendererComponent;
	void BindSymbols(CUnit& unit, CircleRendererComponent);

	struct LineRendererComponent;
	void BindSymbols(CUnit& unit, LineRendererComponent);

	struct CameraComponent;
	void BindSymbols(CUnit& unit, CameraComponent);

	struct ScriptComponent;
	void BindSymbols(CUnit& unit, ScriptComponent);

	struct PerpixelRendererComponent;
	void BindSymbols(CUnit& unit, PerpixelRendererComponent);

	struct BoxColliderComponent;
	void BindSymbols(CUnit& unit, BoxColliderComponent);

	struct PhysicsBodyComponent;
	void BindSymbols(CUnit& unit, PhysicsBodyComponent);

	struct SpriteAnimatorComponent;
	void BindSymbols(CUnit& unit, SpriteAnimatorComponent);

	struct TextComponent;
	void BindSymbols(CUnit& unit, TextComponent);
}