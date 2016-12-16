#include "MattermostWebhooks.h"
#include <codecvt>
#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <future>
#include "StdLogger.h"

using namespace std;
using namespace lospi;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using utility::string_t;
using utility::stringstream_t;

namespace {
	string_t url_decode(const string_t &input) {
		auto ret(input);
		replace(ret.begin(), ret.end(), '+', ' ');
		return uri::decode(ret);
	}

	Message parse_message_from_query_string(const string_t &query_string, const string_t &outgoing_hook_token) {
		auto key_values = uri::split_query(query_string);
		return Message{
			url_decode(key_values[_XPLATSTR("token")]) == outgoing_hook_token,
			stol(url_decode(key_values[_XPLATSTR("timestamp")])),
			url_decode(key_values[_XPLATSTR("channel_name")]),
			url_decode(key_values[_XPLATSTR("team_domain")]),
			url_decode(key_values[_XPLATSTR("text")]),
			url_decode(key_values[_XPLATSTR("user_name")]),
			url_decode(key_values[_XPLATSTR("trigger_word")])
		};
	}
}

MattermostWebhooks::MattermostWebhooks(const string_t &mattermost_url,
	const string_t &incoming_hook_route,
	const string_t &outgoing_hook_route,
	const string_t &outgoing_hook_token)
	:
	outgoing_hook_route(outgoing_hook_route),
	outgoing_hook_token(outgoing_hook_token),
	client(mattermost_url),
	incoming_hook_route(incoming_hook_route),
	is_alive(true),
	message_handler([this](auto message)
		{
		log->info(_XPLATSTR("[+] Received message: ") + message.get_text());
		return _XPLATSTR("Message received.");
	}),
	web_handler([this]() {
		log->info(_XPLATSTR("Handled web request."));
		string_t status = is_alive ? _XPLATSTR("alive") : _XPLATSTR("dead");
		return WebResponse(_XPLATSTR("<b>MattermostWebhooks is ") + status + _XPLATSTR(". </b>"), _XPLATSTR("text/html"));
	}),
	log(make_shared<StdLogger>())
{
}

MattermostWebhooks::~MattermostWebhooks()
{
	die();
	if (listener_thread.joinable()) {
		listener_thread.join();
	}
}

void MattermostWebhooks::post_message(const string_t &message)
{
	log->info(_XPLATSTR("Posting message: ") + message);
	json::value body_data;
	body_data[_XPLATSTR("text")] = json::value::string(message);

	string_t payload(_XPLATSTR("payload="));
	payload.append(uri::encode_data_string(body_data.serialize()));
	
	http_request request;
	request.set_method(methods::POST);
	request.headers().add(_XPLATSTR("Content-Type"), _XPLATSTR("application/x-www-form-urlencoded"));
	request.set_body(payload);
	request.set_request_uri(incoming_hook_route);
	auto request_task = client.request(request);
	
	try
	{
		auto response = request_task.get();
		stringstream_t ss;
		ss << _XPLATSTR("Posted to Incoming Webhook: ") << message << "; response: " << 
			response.status_code() << ". " << response.body() << endl;
		log->info(ss.str());
	}
	catch (http_exception e)
	{
		log->error(_XPLATSTR("An exception occurred posting to incoming webhook"));
		string msg ("There was an issue when POSTing to the Incoming Webhook: ");
		msg.append(e.what());
		throw exception(e);
	}
}

void MattermostWebhooks::register_message_handler(const function<string_t(const Message&)> &message_handler) 
{
	log->error(_XPLATSTR("Registering a message handler."));
	this->message_handler = message_handler;
}

void MattermostWebhooks::register_web_handler(const function<WebResponse()> &web_handler)
{
	log->error(_XPLATSTR("Registering a web handler."));
	this->web_handler = web_handler;
}

void MattermostWebhooks::listen() 
{
	listener_thread = thread([this]() {
		log->info(_XPLATSTR("Starting MattermostWebhooks listener."));
		http_listener listener(outgoing_hook_route);
		try {
			listener.open().wait();
		}
		catch (exception e)
		{
			log->error(_XPLATSTR("An error occurred opening a listening port for Outgoing WebHooks"));
			cerr << e.what() << endl;
			return;
		}
		listener.support(methods::GET, [this](http_request req) {
			auto web_handler_future = async(launch::async, [this, req]() {
				return web_handler();
			});
			try {
				auto web_handler_result = web_handler_future.get();
				req.reply(status_codes::OK, web_handler_result.get_content(), web_handler_result.get_content_type());
			}
			catch (exception e) 
			{
				log->error(_XPLATSTR("Error getting web handler result."));
				req.reply(status_codes::InternalError, e.what());
			}
		});
		listener.support(methods::POST, [this](http_request req) {
			auto body_util = req.extract_string().get();
			auto message = parse_message_from_query_string(body_util, outgoing_hook_token);
			req.reply(status_codes::OK);
			try {
				auto command_result = async(launch::async, [this, message]() {
					return message_handler(message);
				});
				post_message(command_result.get());
			}
			catch (exception e)
			{
				log->error(_XPLATSTR("Exception thrown while posting message."));
			}
		});
		log->info(_XPLATSTR("MattermostWebhooks listening for Outgoing Webhooks from Mattermost."));
		while (is_alive) {
			this_thread::sleep_for(chrono::milliseconds(500));
		}
		log->info(_XPLATSTR("MattermostWebhooks going down."));
		listener.close();
	});
}

void MattermostWebhooks::die() 
{
	log->warn(_XPLATSTR("Matterbot got the kill signal."));
	is_alive.exchange(false);
}

void MattermostWebhooks::set_logger(std::shared_ptr<ILogger> log)
{
	this->log = log;
}
