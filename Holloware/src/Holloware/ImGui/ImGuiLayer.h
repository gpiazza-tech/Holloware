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
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}
