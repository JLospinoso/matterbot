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

class lospi::MatterbotImpl {
public:
	wstring serve_command_from_message(const Message &message);
	wstring get_help_string() const;
	WebResponse get_default_web_response();
	long websites_served = 0, messages_sent = 0, commands_served = 0;
	map<wstring, shared_ptr<ICommand>> commands;
	shared_ptr<ILogger> log;
	unique_ptr<MattermostWebhooks> webhooks;
	atomic<bool> is_alive;
};

WebResponse MatterbotImpl::get_default_web_response() {
	wstring contents(L"<h3>MattermostBot Status</h3>");
	contents.append(L"<p>Web requests served: ");
	contents.append(to_wstring(websites_served));
	contents.append(L"</p><p>Messages posted: ");
	contents.append(to_wstring(messages_sent));
	contents.append(L"</p><p>Commands served: ");
	contents.append(to_wstring(commands_served));
	contents.append(L"</p><h5>Supported commands:</h5><ul>");

	for (auto cmd_pair : commands) {
		contents.append(L"<li>");
		contents.append(cmd_pair.first);
		contents.append(L"</li>");
	}
	contents.append(L"</ul>");

	websites_served++;
	return WebResponse(contents, L"text/html");
}

wstring MatterbotImpl::get_help_string() const {
	wstring help(L"Supported commands\n===\n");
	for (auto cmd_pair : commands) {
		help.append(cmd_pair.second->get_help());
		help.append(L"\n\n");
	}
	return help;
}

wstring MatterbotImpl::serve_command_from_message(const Message &message) {
	commands_served++;
	if (!message.token_is_valid()) {
		return L"Invalid token provided for command.";
	}
	wstringstream text_ss(message.get_text());
	wstring trigger_word, command_string, arguments;
	getline(text_ss, trigger_word, L' ');
	getline(text_ss, command_string, L' ');
	getline(text_ss, arguments);
	if (L"help" == command_string)
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
		return L"Command not found: " + command_string;
	}
	return command_pair->second->handle_command(message.get_team(), message.get_channel(), message.get_user(), arguments);
}


Matterbot::Matterbot(const std::wstring &mattermost_url,
	const std::wstring &incoming_hook_route,
	const std::wstring &outgoing_hook_route,
	const std::wstring &outgoing_hook_token)
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

void Matterbot::post_message(const wstring& message) {
	impl->webhooks->post_message(message);
	impl->messages_sent++;
}

void Matterbot::update_website(const wstring& contents)
{
	impl->webhooks->register_web_handler([contents]() {
		return WebResponse(contents, L"text/html");
	});
}
