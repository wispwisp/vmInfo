#include "Request.hpp"

#include <regex>

Request::Request(const char* requestStr)
  : isValid(false), hypervisor(), domain(), command() {

  static const std::regex re{ R"(^/([^/]+)/([^/]+)/([^/\?]+)(?:\?(.*))?$)" };

  std::cmatch match;
  if (std::regex_match(requestStr, match, re) &&
      !match[1].str().empty() &&
      !match[2].str().empty() &&
      !match[3].str().empty()) {

    isValid = true;
    hypervisor = match[1];
    domain = match[2];
    command = match[3];
    // additional = match[4];
  }
}

std::ostream& operator<< (std::ostream& os, const Request& rhs) {
  os << '/' << rhs.hypervisor
     << '/' << rhs.domain
     << '/' << rhs.command
     << '\n';
  return os;
}
