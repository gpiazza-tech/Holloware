#pragma once

namespace Perplex
{
	class SpriteRegistryPanel
	{
	public:
		void Render();
	private:
		uint32_t m_SelectedAtlas{ 0 };
	};
}