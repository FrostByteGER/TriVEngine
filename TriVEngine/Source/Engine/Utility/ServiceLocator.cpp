#include "ServiceLocator.hpp"
#include "../Messaging/MessageBus.hpp"
#include "ObjectPtr.hpp"


template<class T>
std::enable_if_t<std::is_base_of_v<TriV::Engine::Utility::IEngineService, T>, bool> TriV::Engine::Utility::ServiceLocator::RegisterService()
{
	const ServiceLocator& instance = Instance();
	const auto result = instance._services.find(typeid(T));
	if (result != instance._services.end())
		return false;
	instance._services[typeid(T)] = T();
	return true;
}

template <class T>
std::enable_if_t<std::is_base_of_v<TriV::Engine::Utility::IEngineService, T>, TriV::Engine::Utility::object_ptr<T>> TriV::Engine::Utility::ServiceLocator::GetService()
{
	const auto result = Instance()._services.find(typeid(T));
	const ServiceLocator& instance = Instance();
	if (result == instance._services.end())
		return nullptr;
	return result->second;
	
}

void TriV::Engine::Utility::ServiceLocator::RegisterDefaultServices()
{
	//services[Messaging::IMessageBus]
}
