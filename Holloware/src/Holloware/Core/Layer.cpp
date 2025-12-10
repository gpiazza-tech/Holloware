#include "hwpch.h"
#include "Layer.h"

#include "Holloware/Core/Timestep.h"

namespace Holloware
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::OnUpdate(Timestep ts) {}
}