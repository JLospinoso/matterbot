#pragma once

namespace lospi {
	class ICommand {
	public:
		virtual ~ICommand() {};
		virtual std::wstring get_name() = 0;
		virtual std::wstring get_help() = 0;
		virtual std::wstring handle_command(std::wstring team, std::wstring channel, std::wstring user, std::wstring command_text) = 0;
	};
}
