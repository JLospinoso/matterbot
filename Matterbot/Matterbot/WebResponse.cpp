#include "WebResponse.h"

using namespace lospi;
using namespace std;

WebResponse::WebResponse() {}

WebResponse::WebResponse(const wstring& content, const wstring& content_type) : content_type(content_type), content(content) {}

wstring WebResponse::get_content_type() const { return content_type; }

wstring WebResponse::get_content() const { return content; }
