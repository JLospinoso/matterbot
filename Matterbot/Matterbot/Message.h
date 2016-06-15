#pragma once
#include <string>

namespace lospi {
	class Message {
	public:
		Message();
		Message(const bool &valid_token, const long &timestamp, const std::wstring &channel, const std::wstring &team,
			const std::wstring &text, const std::wstring &user, const std::wstring &trigger_word);
		bool token_is_valid() const;
		long get_timestamp() const;
		std::wstring get_channel() const;
		std::wstring get_team() const;
		std::wstring get_text() const;
		std::wstring get_user() const;
		std::wstring get_trigger_word() const;
	private:
		std::wstring channel, team, text, user, trigger_word;
		long timestamp;
		bool valid_token;
	};
}
