#pragma once
#include "MattermostWebhooks.h"
#include "ICommand.h"

namespace lospi {
	class MattermostBot {
	public:
		MattermostBot(std::unique_ptr<MattermostWebhooks> webhooks);
		void register_command(std::shared_ptr<ICommand> command);
		void unregister_command(std::shared_ptr<ICommand> command);
		void post_message(const std::wstring& message);
		void update_website(const std::wstring& contents);
		std::wstring serve_command_from_message(const Message &message);
	private:
		std::wstring get_help_string() const;
		WebResponse get_default_web_response();
		std::atomic<bool> is_alive;
		std::unique_ptr<MattermostWebhooks> webhooks;
		long websites_served = 0, messages_sent = 0, commands_served = 0;
		std::map<std::wstring, std::shared_ptr<ICommand>> commands;
	};
}