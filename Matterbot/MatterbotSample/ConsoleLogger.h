#pragma once
#include "StdLogger.h"
#include <mutex>

namespace lospi {
  struct ConsoleLogger : ILogger {
    void info(const std::wstring& msg) override;
    void warn(const std::wstring& msg) override;
    void error(const std::wstring& msg) override;
    void write(const std::wstring& type, const std::wstring& msg);
  private:
    std::mutex write_mutex;
  };
}
