#pragma once

#include <Perplex/Events/Event.h>
#include <Perplex/Assets/Asset.h>

namespace Perplex
{
	class AssetImportedEvent : public Event
	{
	public:
		AssetImportedEvent(Asset asset) : m_Asset(asset) { }

		inline Asset GetAsset() const { return m_Asset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AssetImportedEvent: " << m_Asset.GetName();
			return ss.str();
		}

		EVENT_CLASS_TYPE(AssetImported)
		EVENT_CLASS_CATEGORY(EventCategoryAsset)
	private:
		Asset m_Asset;
	};
}