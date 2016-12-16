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
	string_t mattermost_url = U("https://hooks.slack.com"),
		incoming_hook_route = U("services/AAAAAAA/BBBBBBBBB/CCCCCCCCCCCCCCCCCCCC"),
		outgoing_hook_route = U("http://127.0.0.1:8000/"),
		outgoing_hook_token = U("XXXXXXXXXXXXXXXXXXXXX"),
		welcome_message = U("bot is running.");

	try {
		auto bot = make_shared<Matterbot>(mattermost_url, incoming_hook_route, outgoing_hook_route, outgoing_hook_token);
		bot->set_logger(make_unique<FileBackedLogger>());
		bot->register_command(make_shared<EchoCommand>());
		bot->post_message(welcome_message);

		string_t console;
		ucout << ">> Type \'quit\' to quit. Any other input will be sent to the bot." << endl;
		while (getline(ucin, console)) {
			if (U("quit") == console)
			{
				ucout << ">> Quitting." << endl;
				return ERROR_SUCCESS;
			}
			if (U("") != console)
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
