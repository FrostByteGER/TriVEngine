#include "MessageBus.hpp"



TriV::Engine::Core::Messaging::MessageBus::MessageBus()
{

}

void TriV::Engine::Core::Messaging::MessageBus::processMessages()
{
	while (messageQueue.size() > 0)
	{
		messageQueue.pop();
	}
}

void TriV::Engine::Core::Messaging::MessageBus::publishMessage(const Message msg)
{
	messageQueue.push(msg);
}

template<class T>
void TriV::Engine::Core::Messaging::MessageBus::subscribeToMessage(void* function)
{
	
}

template<class T>
void TriV::Engine::Core::Messaging::MessageBus::unsubscribeFromMessage(void* function)
{
}