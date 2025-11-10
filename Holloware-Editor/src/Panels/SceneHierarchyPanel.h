#pragma once

#include "Holloware/Scene/Entity.h"

#include "Holloware/Scene/Scene.h"
#include "Holloware/Core/Core.h"

namespace Holloware
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		template <typename T>
		void DrawComponent(Entity entity, const char* name, void (*DrawBody)(T&));
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}