#include "FileBackedLogger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace lospi;
using utility::string_t;
using utility::stringstream_t;

FileBackedLogger::FileBackedLogger(string file_name) : file(file_name)
{
	if (!file.is_open())
	{
		auto message = "Could not open log at " + file_name;
		throw runtime_error(message.c_str());
	}
}

void FileBackedLogger::info(const string_t &msg) {
	write(U("INFO"), msg);
}

void FileBackedLogger::warn(const string_t &msg) {
	write(U("WARN"), msg);
}

void FileBackedLogger::error(const string_t &msg) {
	write(U("ERROR"), msg);
}

void FileBackedLogger::write(const string_t &type, const string_t &msg)
{
	lock_guard<mutex> lock(write_mutex);
	auto t = time(nullptr);
	auto buffer = localtime(&t);
	stringstream_t ss;
	ss << put_time(buffer, U("%d-%m-%Y %H-%M-%S ")) << type << U(" ") << msg << endl;
	auto log_message = ss.str();
	ucout << log_message;
	file << log_message;
}
