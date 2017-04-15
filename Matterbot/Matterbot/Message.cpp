#include "Message.h"

using namespace std;
using namespace lospi;

Message::Message(const bool& valid_token, const long& timestamp, const wstring& channel, const wstring& team,
                 const wstring& text, const wstring& user, const wstring& trigger_word) :
  channel(channel), team(team), text(text), user(user), trigger_word(trigger_word), timestamp(timestamp), valid_token(valid_token) {}

bool Message::token_is_valid() const { return valid_token; }

long Message::get_timestamp() const { return timestamp; }

wstring Message::get_channel() const { return channel; }

wstring Message::get_team() const { return team; }

wstring Message::get_text() const { return text; }

wstring Message::get_user() const { return user; }

wstring Message::get_trigger_word() const { return trigger_word; }
