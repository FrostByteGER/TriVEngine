#include "MessageBus.hpp"
#include <memory>
#include <algorithm>

void TriV::Engine::Core::Messaging::MessageBus::subscribeToMessage(const type_info& messageType, std::function<void(Message)> function)
{
	/*
	auto pair = messageDictionary.find(messageType);
	
	if (pair != messageDictionary.end())
	{
		pair->second.push_back(function);
	}
	else
	{
		std::vector<std::function<void(Message)>> functions = { function };
		messageDictionary.emplace(messageType, functions);
	}*/
}

void TriV::Engine::Core::Messaging::MessageBus::unsubscribeFromMessage(const type_info& messageType, std::function<void(Message)> function)
{
	/*
	auto pair = messageDictionary.find(messageType);
	if (pair != messageDictionary.end())
	{
		std::vector<std::function<void(Message)>>& list = pair->second;
		list.erase(std::remove_if(list.begin(), list.end(), [function](auto function2) {return function.target<std::function<void(Message)>>() < function2.target<std::function<void(Message)>>(); }), list.end());
	}*/
}

void TriV::Engine::Core::Messaging::MessageBus::publishMessage(std::unique_ptr<Message> msg)
{
	/*
	auto pair = messageDictionary.find(typeid(msg));
	if (pair != messageDictionary.end() && pair->second.size() > 0)
	{
		for (const auto& element : pair->second)
		{
			element(*msg);
		}
	}*/
}

void TriV::Engine::Core::Messaging::MessageBus::cleanUp()
{
	/*
	std::vector<std::type_index> classesToRemove;
	for(auto& pair : messageDictionary)
	{
		std::vector<std::function<void(Message)>> functionsToRemove;
		for (const auto& f : pair.second)
		{
			if (!f)
				functionsToRemove.push_back(f);
		}

		for (const auto& f : functionsToRemove)
		{
			pair.second.erase(std::remove_if(pair.second.begin(), pair.second.end(), [f](auto function2) {return f.target<std::function<void(Message)>>() < function2.target<std::function<void(Message)>>(); }), pair.second.end());
		}

		if (pair.second.size() == 0)
			classesToRemove.push_back(pair.first);
	}

	for (const auto& classToRemove : classesToRemove)
	{
		messageDictionary.erase(std::remove_if(messageDictionary.begin(), messageDictionary.end(), [classToRemove](auto& c2) {return classToRemove == c2.first; }), messageDictionary.end());
	}*/
}
