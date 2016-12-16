#include "Message.h"

using namespace std;
using namespace lospi;
using utility::string_t;

Message::Message()
{
}

Message::Message(const bool &valid_token, const long &timestamp, const string_t &channel, const string_t &team,
	const string_t &text, const string_t &user, const string_t &trigger_word) :
	valid_token(valid_token), timestamp(timestamp), channel(channel), team(team), text(text), user(user), trigger_word(trigger_word) {
}

bool Message::token_is_valid() const {
	return valid_token;
}

long Message::get_timestamp() const {
	return timestamp;
}

string_t Message::get_channel() const {
	return channel;
}

string_t Message::get_team() const {
	return team;
}

string_t Message::get_text() const {
	return text;
}

string_t Message::get_user() const {
	return user;
}

string_t Message::get_trigger_word() const {
	return trigger_word;
}
