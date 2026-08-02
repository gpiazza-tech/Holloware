#pragma once

#include <string_view>
#include <string>
#include <functional>

namespace Perplex
{
	class BoolPopup
	{
	public:
		BoolPopup(const char* label);

		void Open(std::function<void(bool)> closeCallback);
		void Update();
	private:
		const char* m_Label;

		bool m_TriggerOpen;
		std::function<void(bool)> m_CloseCallback;
	};
}