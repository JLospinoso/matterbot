#include "ConsoleLogger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace lospi;

void ConsoleLogger::info(const wstring& msg) { write(L"INFO", msg); }

void ConsoleLogger::warn(const wstring& msg) { write(L"WARN", msg); }

void ConsoleLogger::error(const wstring& msg) { write(L"ERROR", msg); }

void ConsoleLogger::write(const wstring& type, const wstring& msg) {
  lock_guard<mutex> lock(write_mutex);
  struct tm buffer;
  auto t = time(nullptr);
  auto localtime_result = localtime_s(&buffer, &t);
  if (0 != localtime_result) { throw exception("Could not get localtime."); }
  wstringstream ss;
  ss << put_time(&buffer, L"%d-%m-%Y %H-%M-%S ") << type << L" " << msg << endl;
  auto log_message = ss.str();
  wcout << log_message;
}
