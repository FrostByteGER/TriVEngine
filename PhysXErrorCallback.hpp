#pragma once

#include <foundation/PxErrorCallback.h>

namespace TriV::Engine::Physics
{
	class PhysXErrorCallback : public physx::PxErrorCallback
	{
	public:
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
	};
}


