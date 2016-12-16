#pragma once
#include "Matterbot.h"
#include <cpprest/asyncrt_utils.h>

namespace lospi
{
	class EchoCommand : public ICommand {
	public:
		utility::string_t get_name() override {
			return _XPLATSTR("echo");
		}

		utility::string_t get_help() override {
			return _XPLATSTR("`echo [MESSAGE]`: `echo` will respond with whatever message you give it.");
		}

		utility::string_t handle_command(const utility::string_t &team, const utility::string_t &channel,
			const utility::string_t &user, const utility::string_t &command_text) override {
			return command_text;
		}
	};
}
