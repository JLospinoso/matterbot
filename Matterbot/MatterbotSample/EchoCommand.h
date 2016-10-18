#pragma once
#include "Matterbot.h"

namespace lospi
{
	class EchoCommand : public ICommand {
	public:
		std::wstring get_name() override {
			return L"echo";
		}

		std::wstring get_help() override {
			return L"`echo [MESSAGE]`: `echo` will respond with whatever message you give it.";
		}

		std::wstring handle_command(const std::wstring &team, const std::wstring &channel,
			const std::wstring &user, const std::wstring &command_text) override {
			return command_text;
		}
	};
}
