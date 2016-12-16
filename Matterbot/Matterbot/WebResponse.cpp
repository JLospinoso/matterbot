#include "WebResponse.h"

using namespace lospi;
using namespace std;
using utility::string_t;

WebResponse::WebResponse()
{
}

WebResponse::WebResponse(const string_t &content, const string_t &content_type) : content(content), content_type(content_type)
{
}

string_t WebResponse::get_content_type() const
{
	return content_type;
}

string_t WebResponse::get_content() const
{
	return content;
}
