#pragma once
#include "StdLogger.h"
#include <mutex>
#include <fstream>

namespace lospi
{
	struct FileBackedLogger : public ILogger {
		explicit FileBackedLogger(std::string file_name = "log.txt");
		void info(const std::wstring &msg) override;
		void warn(const std::wstring &msg) override;
		void error(const std::wstring &msg) override;
		void write(const std::wstring &type, const std::wstring &msg);
	private:
		std::wofstream file;
		std::mutex write_mutex;
	};
}
