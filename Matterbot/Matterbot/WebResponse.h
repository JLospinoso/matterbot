#pragma once
#include <string>

namespace lospi {
  class WebResponse {
  public:
    WebResponse();
    WebResponse(const std::wstring& content, const std::wstring& content_type = L"text/html");
    std::wstring get_content_type() const;
    std::wstring get_content() const;
  private:
    std::wstring content_type;
    std::wstring content;
  };
}
