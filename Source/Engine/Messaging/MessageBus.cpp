#include "MessageBus.hpp"
#include <memory>


TriV::Engine::Core::Messaging::MessageBus::MessageBus()
{
}

void TriV::Engine::Core::Messaging::MessageBus::processMessages()
{
	while (subscriptions.size() > 0)
	{
		//subscriptions.pop();
	}
}

template<class T>
void TriV::Engine::Core::Messaging::MessageBus::publishMessage(std::unique_ptr<T> msg)
{
	const MessageMap_t::const_iterator result = subscriptions.find(typeid(T));
	if (result == subscriptions.end())
		return;

	for (IMessageSubscription* e : result->second)
	{
		e->deliver(msg);
	}
}

template<class T>
void TriV::Engine::Core::Messaging::MessageBus::subscribeToMessage(std::function<void(T)> function)
{
	if (!function)
		return; //TODO: Add Exception
	const MessageMap_t::const_iterator result = subscriptions.find(typeid(T));
	std::unique_ptr<MessageTokens_t> tokens = nullptr;
	if(result == subscriptions.end())
	{
		tokens = std::make_unique<MessageTokens_t>();
		//subscriptions[typeid(T)] = tokens;
	}
	//tokens = result->second;
	//tokens->push_back() TODO: Insert MessageSubscription here!

}

template<class T>
void TriV::Engine::Core::Messaging::MessageBus::unsubscribeFromMessage(std::function<void(T)> function)
{
}