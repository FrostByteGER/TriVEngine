#pragma once
#include <map>
#include "IEngineService.hpp"

namespace TriV::Engine::Core::Services
{
	class ServiceLocator
	{
	public:
		template<class T>
		bool registerService();
		void registerDefaultServices();
	private:
		std::map<type_info, IEngineService> services;
	};

}
