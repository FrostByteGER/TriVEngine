#include "ServiceLocator.hpp"
#include "../Messaging/MessageBus.hpp"


template<class T>
bool TriV::Engine::Core::Services::ServiceLocator::registerService()
{
	const std::map<type_info, IEngineService>::const_iterator result = services.find(typeid(T));
	if (result != services.end())
		return false;
	services[typeid(T)] = T();
	return true;
}

void TriV::Engine::Core::Services::ServiceLocator::registerDefaultServices()
{
	//services[Messaging::IMessageBus]
}
