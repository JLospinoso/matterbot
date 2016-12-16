#include "Matterbot.h"
#include "MattermostWebhooks.h"
#include "StdLogger.h"
#include "Message.h"
#include <atomic>
#include <map>
#include <sstream>
#include <functional>

using namespace lospi;
using namespace std;
using utility::string_t;
using utility::stringstream_t;
using utility::conversions::to_string_t;

class lospi::MatterbotImpl {
public:
	string_t serve_command_from_message(const Message &message);
	string_t get_help_string() const;
	WebResponse get_default_web_response();
	long websites_served = 0, messages_sent = 0, commands_served = 0;
	map<string_t, shared_ptr<ICommand>> commands;
	shared_ptr<ILogger> log;
	unique_ptr<MattermostWebhooks> webhooks;
	atomic<bool> is_alive;
};

WebResponse MatterbotImpl::get_default_web_response() {
	string_t contents(_XPLATSTR("<h3>MattermostBot Status</h3>"));
	contents.append(_XPLATSTR("<p>Web requests served: "));
	contents.append(to_string_t(to_string(websites_served)));
	contents.append(_XPLATSTR("</p><p>Messages posted: "));
	contents.append(to_string_t(to_string(messages_sent)));
	contents.append(_XPLATSTR("</p><p>Commands served: "));
	contents.append(to_string_t(to_string(commands_served)));
	contents.append(_XPLATSTR("</p><h5>Supported commands:</h5><ul>"));

	for (auto cmd_pair : commands) {
		contents.append(_XPLATSTR("<li>"));
		contents.append(cmd_pair.first);
		contents.append(_XPLATSTR("</li>"));
	}
	contents.append(_XPLATSTR("</ul>"));

	websites_served++;
	return WebResponse(contents, _XPLATSTR("text/html"));
}

string_t MatterbotImpl::get_help_string() const {
	string_t help(_XPLATSTR("Supported commands\n===\n"));
	for (auto cmd_pair : commands) {
		help.append(cmd_pair.second->get_help());
		help.append(_XPLATSTR("\n\n"));
	}
	return help;
}

string_t MatterbotImpl::serve_command_from_message(const Message &message) {
	commands_served++;
	if (!message.token_is_valid()) {
		return _XPLATSTR("Invalid token provided for command.");
	}
	stringstream_t text_ss(message.get_text());
	string_t trigger_word, command_string, arguments;
	getline(text_ss, trigger_word, ' ');
	getline(text_ss, command_string, ' ');
	getline(text_ss, arguments);
	if (to_string_t("help") == command_string)
	{
		auto command_pair = commands.find(arguments);
		if (command_pair == commands.end())
		{
			return get_help_string();
		}
		return command_pair->second->get_help();
	}
	auto command_pair = commands.find(command_string);
	if (command_pair == commands.end())
	{
		return _XPLATSTR("Command not found: ") + command_string;
	}
	return command_pair->second->handle_command(message.get_team(), message.get_channel(), message.get_user(), arguments);
}


Matterbot::Matterbot(const string_t &mattermost_url,
	const string_t &incoming_hook_route,
	const string_t &outgoing_hook_route,
	const string_t &outgoing_hook_token)
	: impl(make_shared<MatterbotImpl>())
{
	impl->log = make_unique<StdLogger>();
	impl->webhooks = make_unique<MattermostWebhooks>(mattermost_url, incoming_hook_route, outgoing_hook_route, outgoing_hook_token);
	impl->is_alive = true;
	impl->webhooks->register_web_handler(bind(&MatterbotImpl::get_default_web_response, impl));
	auto impl_copy = impl;
	impl->webhooks->register_message_handler([impl_copy](const Message& message) {
		return impl_copy->serve_command_from_message(message);
	});
	impl->webhooks->listen();
}


Matterbot::~Matterbot() { }

void Matterbot::set_logger(shared_ptr<ILogger> log) {
	impl->log = log;
	impl->webhooks->set_logger(log);
}

void Matterbot::register_command(shared_ptr<ICommand> command)
{
	impl->commands[command->get_name()] = command;
}

void Matterbot::unregister_command(shared_ptr<ICommand> command)
{
	impl->commands.erase(command->get_name());
}

void Matterbot::post_message(const string_t& message) {
	impl->webhooks->post_message(message);
	impl->messages_sent++;
}

void Matterbot::update_website(const string_t& contents)
{
	impl->webhooks->register_web_handler([contents]() {
		return WebResponse(contents, _XPLATSTR("text/html"));
	});
}
