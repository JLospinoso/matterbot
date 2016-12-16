#pragma once
#include <string>
#include <cpprest/asyncrt_utils.h>

namespace lospi {
	class Message {
	public:
		Message();
		Message(const bool &valid_token, const long &timestamp, const utility::string_t &channel, const utility::string_t &team,
			const utility::string_t &text, const utility::string_t &user, const utility::string_t &trigger_word);
		bool token_is_valid() const;
		long get_timestamp() const;
		utility::string_t get_channel() const;
		utility::string_t get_team() const;
		utility::string_t get_text() const;
		utility::string_t get_user() const;
		utility::string_t get_trigger_word() const;
	private:
		utility::string_t channel, team, text, user, trigger_word;
		long timestamp;
		bool valid_token;
	};
}
