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

namespace {
  wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;

  wstring url_decode(const wstring& input) {
    auto ret(input);
    replace(ret.begin(), ret.end(), L'+', L' ');
    return uri::decode(ret);
  }

  Message parse_message_from_query_string(const wstring& query_string, const wstring& outgoing_hook_token) {
    auto key_values = uri::split_query(query_string);
    return Message{
      url_decode(key_values[L"token"]) == outgoing_hook_token,
      stol(url_decode(key_values[L"timestamp"])),
      url_decode(key_values[L"channel_name"]),
      url_decode(key_values[L"team_domain"]),
      url_decode(key_values[L"text"]),
      url_decode(key_values[L"user_name"]),
      url_decode(key_values[L"trigger_word"])
    };
  }
}

MattermostWebhooks::MattermostWebhooks(const wstring& mattermost_url,
                                       const wstring& incoming_hook_route,
                                       const wstring& outgoing_hook_route,
                                       const wstring& outgoing_hook_token)
  :
  outgoing_hook_route(outgoing_hook_route),
  outgoing_hook_token(outgoing_hook_token),
  incoming_hook_route(incoming_hook_route),
  client(mattermost_url),
  is_alive(true),
  message_handler([](auto message) {
      wclog << L"[+] Received message: " + message.get_text() << endl;
      return L"Message received.";
    }),
  web_handler([this]() {
      log->info(L"Handled web request.");
      wstring status = is_alive ? L"alive" : L"dead";
      return WebResponse(L"<b>MattermostWebhooks is " + status + L". </b>", L"text/html");
    }),
  log(make_shared<StdLogger>()) {}

MattermostWebhooks::~MattermostWebhooks() {
  die();
  if (listener_thread.joinable()) { listener_thread.join(); }
}

void MattermostWebhooks::post_message(const wstring& message) {
  log->info(L"Posting message: " + message);
  json::value body_data;
  body_data[L"text"] = json::value::string(message);

  wstring payload(L"payload=");
  wstringstream ss;
  ss << body_data;
  payload.append(uri::encode_data_string(ss.str()));

  http_request request;
  request.set_method(methods::POST);
  request.headers().add(L"Content-Type", L"application/x-www-form-urlencoded");
  request.set_body(payload);
  request.set_request_uri(incoming_hook_route);
  auto request_task = client.request(request);

  try {
    auto response = request_task.get();
    wstringstream ss;
    ss << L"Posted to Incoming Webhook: " << message << "; response: " <<
      response.status_code() << ". " << response.body() << endl;
    log->info(ss.str());
  }
  catch (http_exception e) {
    log->error(L"An exception occurred posting to incoming webhook");
    string msg("There was an issue when POSTing to the Incoming Webhook: ");
    msg.append(e.what());
    throw exception(e);
  }
}

void MattermostWebhooks::register_message_handler(const function<wstring(const Message&)>& message_handler) {
  log->error(L"Registering a message handler.");
  this->message_handler = message_handler;
}

void MattermostWebhooks::register_web_handler(const function<WebResponse()>& web_handler) {
  log->error(L"Registering a web handler.");
  this->web_handler = web_handler;
}

void MattermostWebhooks::listen() {
  listener_thread = thread([this]() {
      log->info(L"Starting MattermostWebhooks listener.");
      http_listener listener(outgoing_hook_route);
      try { listener.open().wait(); }
      catch (exception e) {
        log->error(L"An error occurred opening a listening port for Outgoing WebHooks");
        cerr << e.what() << endl;
        return;
      }
      listener.support(methods::GET, [this](http_request req) {
                         auto web_handler_future = async(launch::async, [this, req]() { return web_handler(); });
                         try {
                           auto web_handler_result = web_handler_future.get();
                           req.reply(status_codes::OK, web_handler_result.get_content(), web_handler_result.get_content_type());
                         }
                         catch (exception e) {
                           log->error(L"Error getting web handler result.");
                           req.reply(status_codes::InternalError, e.what());
                         }
                       });
      listener.support(methods::POST, [this](http_request req) {
                         auto body_util = req.extract_string().get();
                         auto body = string(body_util.begin(), body_util.end());
                         auto body_w = converter.from_bytes(body);
                         auto message = parse_message_from_query_string(body_w, outgoing_hook_token);
                         req.reply(status_codes::OK);
                         try {
                           auto command_result = async(launch::async, [this, message]() { return message_handler(message); });
                           post_message(command_result.get());
                         }
                         catch (exception e) { log->error(L"Exception thrown while posting message."); }
                       });
      log->info(L"MattermostWebhooks listening for Outgoing Webhooks from Mattermost.");
      while (is_alive) { this_thread::sleep_for(chrono::milliseconds(500)); }
      log->info(L"MattermostWebhooks going down.");
      listener.close();
    });
}

void MattermostWebhooks::die() {
  log->warn(L"Matterbot got the kill signal.");
  is_alive.exchange(false);
}

void MattermostWebhooks::set_logger(std::shared_ptr<ILogger> log) { this->log = log; }
