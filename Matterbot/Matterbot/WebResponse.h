#pragma once
#include <string>
#include <cpprest/asyncrt_utils.h>

namespace lospi {
	class WebResponse {
	public:
		WebResponse();
		WebResponse(const utility::string_t &content, const utility::string_t &content_type = U("text/html"));
		utility::string_t get_content_type() const;
		utility::string_t get_content() const;
	private:
		utility::string_t content_type;
		utility::string_t content;
	};
}
