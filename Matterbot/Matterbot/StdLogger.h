#pragma once
#include "Matterbot.h"

namespace lospi {
	class StdLogger : public ILogger {
	public:
		void info(const std::wstring &msg) override;
		void warn(const std::wstring &msg) override;
		void error(const std::wstring &msg) override;
	};
}
