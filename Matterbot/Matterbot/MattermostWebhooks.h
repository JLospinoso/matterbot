#pragma once
#include <thread>
#include <cpprest/http_client.h>
#include <cpprest/asyncrt_utils.h>
#include "WebResponse.h"
#include "Message.h"
#include "Matterbot.h"

namespace lospi {

	class MattermostWebhooks
	{
	public:
		MattermostWebhooks(const utility::string_t &mattermost_url,
			const utility::string_t &incoming_hook_route,
			const utility::string_t &outgoing_hook_route,
			const utility::string_t &outgoing_hook_token);
		~MattermostWebhooks();
		void post_message(const utility::string_t &message);
		void register_message_handler(const std::function<utility::string_t(const Message&)> &message_handler);
		void register_web_handler(const std::function<WebResponse()> &web_handler);
		void listen();
		void die();
		void set_logger(std::shared_ptr<ILogger> log);
	private:
		const utility::string_t outgoing_hook_route, outgoing_hook_token, incoming_hook_route;
		web::http::client::http_client client;
		std::atomic<bool> is_alive;
		std::thread listener_thread;
		std::function<utility::string_t(const Message&)> message_handler;
		std::function<WebResponse()> web_handler;
		std::shared_ptr<ILogger> log;
	};
}
