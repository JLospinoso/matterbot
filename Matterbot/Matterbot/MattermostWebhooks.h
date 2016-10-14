#pragma once
#include <cpprest/http_client.h>
#include "WebResponse.h"
#include "Message.h"

namespace lospi {
	class MattermostWebhooks
	{
	public:
		MattermostWebhooks(const std::wstring &mattermost_url,
			const std::wstring &incoming_hook_route,
			const std::wstring &outgoing_hook_route,
			const std::wstring &outgoing_hook_token);
		~MattermostWebhooks();
		void post_message(const std::wstring &message);
		void register_message_handler(const std::function<std::wstring(const Message&)> &message_handler);
		void register_web_handler(const std::function<WebResponse()> &web_handler);
		void listen();
		void die();
	private:
		const std::wstring outgoing_hook_route, outgoing_hook_token, incoming_hook_route;
		web::http::client::http_client client;
		std::atomic<bool> is_alive;
		std::thread listener_thread;
		std::function<std::wstring(const Message&)> message_handler;
		std::function<WebResponse()> web_handler;
	};
}
