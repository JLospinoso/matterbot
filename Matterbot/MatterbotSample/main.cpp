#include "Matterbot.h"
#include <iostream>
#include <sstream>
#include "FileBackedLogger.h"
#include "EchoCommand.h"

#define ERROR_SUCCESS 0
#define ERROR_FAILURE -1

using namespace std;
using namespace lospi;
using utility::string_t;

int main() {
	string_t mattermost_url = _XPLATSTR("https://hooks.slack.com"),
		incoming_hook_route = _XPLATSTR("services/AAAAAAA/BBBBBBBBB/CCCCCCCCCCCCCCCCCCCC"),
		outgoing_hook_route = _XPLATSTR("http://127.0.0.1:8000/"),
		outgoing_hook_token = _XPLATSTR("XXXXXXXXXXXXXXXXXXXXX"),
		welcome_message = _XPLATSTR("bot is running.");

	try {
		auto bot = make_shared<Matterbot>(mattermost_url, incoming_hook_route, outgoing_hook_route, outgoing_hook_token);
		bot->set_logger(make_unique<FileBackedLogger>());
		bot->register_command(make_shared<EchoCommand>());
		bot->post_message(welcome_message);

		string_t console;
		ucout << ">> Type \'quit\' to quit. Any other input will be sent to the bot." << endl;
		while (getline(ucin, console)) {
			if (_XPLATSTR("quit") == console)
			{
				ucout << ">> Quitting." << endl;
				return ERROR_SUCCESS;
			}
			if (_XPLATSTR("") != console)
			{
				bot->post_message(console);
			}
		}
	}
	catch (exception e)
	{
		ucerr << "[-] Unhandled exception:" << e.what() << endl;
		return ERROR_FAILURE;
	}
}
