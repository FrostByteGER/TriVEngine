#pragma once
#include "Message.hpp"
#include <map>
#include <memory>
#include <functional>
#include <typeindex>

namespace TriV::Engine::Core::Messaging
{


	class IMessageBus
	{
		virtual ~IMessageBus() = default;
		virtual void subscribeToMessage(const type_info& messageType, std::function<void(Message)> function) = 0;
		virtual void unsubscribeFromMessage(const type_info& messageType, std::function<void(Message)> function) = 0;
		virtual void publishMessage(std::unique_ptr<Message> msg) = 0;
		virtual void cleanUp() = 0;
	};

	class MessageBus : IMessageBus
	{
	private:
		~MessageBus() override;

	public:
		void subscribeToMessage(const type_info& messageType, std::function<void(Message)> function) override;
		void unsubscribeFromMessage(const type_info& messageType, std::function<void(Message)> function) override;
		void publishMessage(std::unique_ptr<Message> msg) override;
		void cleanUp() override;

	private:
		std::map<std::type_index, std::vector<std::function<void(Message)>>> messageDictionary;
	};


}
