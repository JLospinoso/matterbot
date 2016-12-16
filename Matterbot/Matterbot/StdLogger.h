#pragma once
#include "Matterbot.h"
#include <cpprest/asyncrt_utils.h>

namespace lospi {
	class StdLogger : public ILogger {
	public:
		void info(const utility::string_t &msg) override;
		void warn(const utility::string_t &msg) override;
		void error(const utility::string_t &msg) override;
	};
}
