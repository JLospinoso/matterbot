#pragma once
#include <memory>
#include <string>

namespace lospi {
	class ILogger {
	public:
		virtual ~ILogger() {};
		virtual void info(const std::wstring &msg) = 0;
		virtual void warn(const std::wstring &msg) = 0;
		virtual void error(const std::wstring &msg) = 0;
	};

	class ICommand {
	public:
		virtual ~ICommand() {};
		virtual std::wstring get_name() = 0;
		virtual std::wstring get_help() = 0;
		virtual std::wstring handle_command(std::wstring team, std::wstring channel, std::wstring user, std::wstring command_text) = 0;
	};

	class MatterbotImpl;
	class Matterbot {
	public:
		Matterbot(const std::wstring &mattermost_url,
			const std::wstring &incoming_hook_route,
			const std::wstring &outgoing_hook_route,
			const std::wstring &outgoing_hook_token);
		~Matterbot();
		Matterbot(const Matterbot&) = delete;
		Matterbot& operator=(const Matterbot&) = delete;
		Matterbot(Matterbot&&) = delete;
		Matterbot& operator=(Matterbot&&) = delete;

		void set_logger(std::unique_ptr<ILogger> log);
		void register_command(std::shared_ptr<ICommand> command);
		void unregister_command(std::shared_ptr<ICommand> command);
		void post_message(const std::wstring& message);
		void update_website(const std::wstring& contents);
	private:
		std::shared_ptr<MatterbotImpl> impl;
	};
}