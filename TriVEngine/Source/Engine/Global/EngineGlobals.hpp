#pragma once

#ifdef EXPORT_ENGINE
#define TRIV_API __declspec(dllexport)
#define TRIV_C_EXPORT extern "C"
#define TRIV_CALL_CONV __cdecl
#else
#define TRIV_API
#endif