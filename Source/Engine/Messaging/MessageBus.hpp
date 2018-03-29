#pragma once
#include <queue>
#include "Message.hpp"
#include <map>
#include <memory>
#include <functional>

namespace TriV::Engine::Core::Messaging
{

	class IMessageSubscription
	{
	public:
		virtual ~IMessageSubscription(){};
		virtual void deliver(Message msg) = 0;
	};

	template<class T>
	class MessageSubscription : public IMessageSubscription
	{
	private:
		std::function<void(T)> function;

	public:
		void deliver(Message msg) override;
	};

	class MessageToken
	{
	public:
		IMessageSubscription* message;
	};


	class MessageBus
	{
	public:
		MessageBus();
		~MessageBus() = default;

		void processMessages();
		template<class T> void publishMessage(std::unique_ptr<T> msg);
		template <class T> void subscribeToMessage(std::function<void(T)> function);
		template<class T> void unsubscribeFromMessage(std::function<void(T)> function);

	private:
		typedef std::vector<std::unique_ptr<IMessageSubscription>> MessageTokens_t;
		typedef std::map<type_info, MessageTokens_t> MessageMap_t;
		MessageMap_t subscriptions;

	};


}
