#ifndef MY_URL
#define MY_URL

#include <regex>
#include <string>
#include <stdexcept>

class Url {
  std::string m_host;
  std::string m_res;
  std::string m_port;
public:
  explicit Url(const std::string& url)
    : m_host(), m_res(), m_port() {

    std::regex re{ R"(^(?:http:/+)?([^/:]+)(?::(\d+))?(/.*)?$)" };
    std::smatch match;

    if (!std::regex_match(url, match, re)) {
      throw std::runtime_error("Invalid url");
    }
    
    m_host = match[1];
    m_port = match[2].str().empty() ? "80" : match[2].str();
    m_res = match[3].str().empty() ? "/" : match[3].str();
  }

  const std::string& host() const noexcept { return m_host; }
  const std::string& resource() const noexcept { return m_res; }
  const std::string& port() const noexcept { return m_port; }
};

#endif // MY_URL
