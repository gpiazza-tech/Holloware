#pragma once

#include "Holloware/Core/Layer.h"

#include "Holloware/Events/ApplicationEvent.h"
#include "Holloware/Events/KeyEvent.h"
#include "Holloware/Events/MouseEvent.h"


namespace Holloware
{
	class HOLLOWARE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}
