#pragma once


namespace TriV::Engine::Core::Messaging
{
	class Message
	{
	public:
		Message();
		virtual ~Message() = default;
	};
}