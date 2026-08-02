#include <Perplex/pch.h>
#include <Perplex/ImGui/BoolPopup.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Perplex
{
	BoolPopup::BoolPopup(const char* label) : m_Label{ label }, m_TriggerOpen{ false } {}

	void BoolPopup::Open(std::function<void(bool)> closeCallback)
	{
		m_CloseCallback = closeCallback;
		m_TriggerOpen = true;
	}

	void BoolPopup::Update()
	{
		if (m_TriggerOpen)
		{
			ImGui::OpenPopup(m_Label);
			m_TriggerOpen = false;
		}

		if (ImGui::BeginPopupModal(m_Label, NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::IsWindowAppearing())
				ImGui::SetKeyboardFocusHere(0);

			if (ImGui::Button("Confirm", ImVec2(120, 0))) 
			{
				m_CloseCallback(true);
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				m_CloseCallback(false);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::IsKeyDown(ImGuiKey_Escape))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}
	}
}