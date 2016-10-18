#include "FileBackedLogger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace lospi;

FileBackedLogger::FileBackedLogger(string file_name) : file(file_name)
{
	if (!file.is_open())
	{
		auto message = "Could not open log at " + file_name;
		throw exception(message.c_str());
	}
}

void FileBackedLogger::info(const wstring &msg) {
	write(L"INFO", msg);
}

void FileBackedLogger::warn(const wstring &msg) {
	write(L"WARN", msg);
}

void FileBackedLogger::error(const wstring &msg) {
	write(L"ERROR", msg);
}

void FileBackedLogger::write(const wstring &type, const wstring &msg)
{
	lock_guard<mutex> lock(write_mutex);
	struct tm buffer;
	auto t = time(nullptr);
	auto localtime_result = localtime_s(&buffer, &t);
	if (0 != localtime_result)
	{
		throw exception("Could not get localtime.");
	}
	wstringstream ss;
	ss << put_time(&buffer, L"%d-%m-%Y %H-%M-%S ") << type << L" " << msg << endl;
	auto log_message = ss.str();
	wcout << log_message;
	file << log_message;
}