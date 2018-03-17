#pragma once
#include <queue>
#include "Message.hpp"

namespace TriV::Engine::Core::Messaging
{
	class MessageBus
	{
	public:
		MessageBus();
		~MessageBus() = default;

		void processMessages();
		void publishMessage(Message msg);
		template <class T> void subscribeToMessage(void* function);
		template<class T> void unsubscribeFromMessage(void* function);

	private:
		std::queue<Message> messageQueue;

	};


}
