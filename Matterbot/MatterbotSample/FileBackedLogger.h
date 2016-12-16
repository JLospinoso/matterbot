#pragma once
#include "StdLogger.h"
#include <mutex>
#include <fstream>

namespace lospi
{
	struct FileBackedLogger : public ILogger {
		explicit FileBackedLogger(std::string file_name = "log.txt");
		void info(const utility::string_t &msg) override;
		void warn(const utility::string_t &msg) override;
		void error(const utility::string_t &msg) override;
		void write(const utility::string_t &type, const utility::string_t &msg);
	private:
		utility::ofstream_t file;
		std::mutex write_mutex;
	};
}
