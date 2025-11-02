#include "hwpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Holloware
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}