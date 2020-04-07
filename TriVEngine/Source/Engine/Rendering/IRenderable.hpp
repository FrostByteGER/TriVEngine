#pragma once

namespace TriV::Engine::Core
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() = default;
		virtual void Render() = 0;
	};
}