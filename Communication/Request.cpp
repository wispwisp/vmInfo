#include "Request.hpp"

#include <regex>

Request::Request(const char* requestStr)
  : isValid(false),
    metod(), hypervisor(), domain(), command(), httpVersion() {

  static const std::regex re {
    R"(^(GET|POST|PUT|DELETE) /([^/]+)/([^/]+)/([^/\? ]+)(?:\?[^ ]+)? (HTTP/\d\.\d)\r?\n[\s\S]*$)" };

  std::cmatch match;
  if (std::regex_match(requestStr, match, re) &&
      !match[1].str().empty() &&
      !match[2].str().empty() &&
      !match[3].str().empty() &&
      !match[4].str().empty() &&
      !match[5].str().empty()) {

    isValid = true;
    metod = match[1];
    hypervisor = match[2];
    domain = match[3];
    command = match[4];
    httpVersion = match[5];
  }
}

std::ostream& operator<< (std::ostream& os, const Request& rhs) {
  os << rhs.metod
     << ' ' << rhs.hypervisor
     << '/' << rhs.domain
     << '/' << rhs.command
     << ' ' << rhs.httpVersion;
  return os;
}
