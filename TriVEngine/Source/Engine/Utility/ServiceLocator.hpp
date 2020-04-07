#pragma once
#include <map>
#include "IEngineService.hpp"
#include "ObjectPtr.hpp"

namespace TriV::Engine::Utility
{
	class ServiceLocator
	{
	public:
		ServiceLocator(const ServiceLocator& other) = delete;
		ServiceLocator(ServiceLocator&& other) noexcept = delete;
		ServiceLocator& operator=(const ServiceLocator& other) = delete;
		ServiceLocator& operator=(ServiceLocator&& other) noexcept = delete;
		
		template<class T>
		static std::enable_if_t<std::is_base_of_v<IEngineService, T>, bool> RegisterService();
		void RegisterDefaultServices();
		template<class T>
		static std::enable_if_t<std::is_base_of_v<IEngineService, T>, object_ptr<T>> GetService();

		// Instance generation and retrieval
		static ServiceLocator& Instance()
		{
			static ServiceLocator instance;
			return instance;
		}
		
	private:
		std::map<type_info, std::unique_ptr<IEngineService>> _services;
		ServiceLocator() = default;
	};

}
