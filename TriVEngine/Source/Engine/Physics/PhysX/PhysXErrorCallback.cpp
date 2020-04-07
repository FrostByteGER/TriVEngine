#include "PhysXErrorCallback.hpp"
#include <iostream>

void TriV::Engine::Physics::PhysX::PhysXErrorCallback::reportError(const physx::PxErrorCode::Enum code, const char* message,
	const char* file, const int line)
{
	const char* errorCode = nullptr;

	switch (code)
	{
	case physx::PxErrorCode::eNO_ERROR:
		errorCode = "no error";
		break;
	case physx::PxErrorCode::eINVALID_PARAMETER:
		errorCode = "invalid parameter";
		break;
	case physx::PxErrorCode::eINVALID_OPERATION:
		errorCode = "invalid operation";
		break;
	case physx::PxErrorCode::eOUT_OF_MEMORY:
		errorCode = "out of memory";
		break;
	case physx::PxErrorCode::eDEBUG_INFO:
		errorCode = "info";
		break;
	case physx::PxErrorCode::eDEBUG_WARNING:
		errorCode = "warning";
		break;
	case physx::PxErrorCode::ePERF_WARNING:
		errorCode = "performance warning";
		break;
	case physx::PxErrorCode::eABORT:
		errorCode = "abort";
		break;
	case physx::PxErrorCode::eINTERNAL_ERROR:
		errorCode = "internal error";
		break;
	case physx::PxErrorCode::eMASK_ALL:
		errorCode = "unknown error";
		break;
	}

	std::cout << "PhysX ERROR::" << errorCode << message << file << line << std::endl;
}