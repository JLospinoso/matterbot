#include "MattermostBot.h"

MattermostBot::MattermostBot(unique_ptr<MattermostWebhooks> webhooks_in) : 
	webhooks(move(webhooks_in)),
	is_alive(true)
{
	webhooks->register_web_handler(bind(&MattermostBot::get_default_web_response, this));
	//TODO: What's going on here?webhooks->register_message_handler(bind(&MattermostBot::serve_command_from_message, this));
	webhooks->register_message_handler([this](const Message& message) {
		return serve_command_from_message(message);
	});
	webhooks->listen();
}

void MattermostBot::register_command(shared_ptr<ICommand> command)
{
	commands[command->get_name()] = command;
}

void MattermostBot::unregister_command(shared_ptr<ICommand> command)
{
	commands.erase(command->get_name());
}

void MattermostBot::post_message(const std::wstring& message) {
	webhooks->post_message(message);
	messages_sent++;
}

void MattermostBot::update_website(const wstring& contents)
{
	webhooks->register_web_handler([contents]() {
		return WebResponse(contents, L"text/html");
	});
}

wstring MattermostBot::serve_command_from_message(const Message &message) {
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

WebResponse MattermostBot::get_default_web_response() {
	time_t now = time(0);
	utility::stringstream_t time_string;
	time_string << put_time(localtime(&now), L"%Y-%m-%dT%H:%M:%SZ");

	wstring contents(L"<h3>MattermostBot Status</h3><p>Current time: ");
	contents.append(time_string.str());
	contents.append(L"</p><p>Web requests served: ");
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

wstring MattermostBot::get_help_string() const {
	wstring help(L"MattermostBot - Supported commands\n===\n");
	for (auto cmd_pair : commands) {
		help.append(cmd_pair.second->get_help());
	}
	return help;
}
