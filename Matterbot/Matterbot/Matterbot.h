#pragma once
#include <memory>
#include <string>
#include <cpprest/asyncrt_utils.h>

namespace lospi {
	class ILogger {
	public:
		virtual ~ILogger() {};
		virtual void info(const utility::string_t &msg) = 0;
		virtual void warn(const utility::string_t &msg) = 0;
		virtual void error(const utility::string_t &msg) = 0;
	};

	class ICommand {
	public:
		virtual ~ICommand() {};
		virtual utility::string_t get_name() = 0;
		virtual utility::string_t get_help() = 0;
		virtual utility::string_t handle_command(const utility::string_t &team, 
				const utility::string_t &channel, 
				const utility::string_t &user, 
				const utility::string_t &command_text) = 0;
	};

	class MatterbotImpl;
	class Matterbot {
	public:
		Matterbot(const utility::string_t &mattermost_url,
			const utility::string_t &incoming_hook_route,
			const utility::string_t &outgoing_hook_route,
			const utility::string_t &outgoing_hook_token);
		~Matterbot();
		Matterbot(const Matterbot&) = delete;
		Matterbot& operator=(const Matterbot&) = delete;
		Matterbot(Matterbot&&) = delete;
		Matterbot& operator=(Matterbot&&) = delete;

		void set_logger(std::shared_ptr<ILogger> log);
		void register_command(std::shared_ptr<ICommand> command);
		void unregister_command(std::shared_ptr<ICommand> command);
		void post_message(const utility::string_t &message);
		void update_website(const utility::string_t &contents);
	private:
		std::shared_ptr<MatterbotImpl> impl;
	};
}
