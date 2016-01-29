#include "Responce.hpp"

#include <string>
#include <cstring>
#include <iterator>
#include <algorithm>
#include <stdexcept>

namespace Tools {
  void makeEntry(std::unordered_map<std::string, std::string>& headers,
		 const char* begin, const char* end) {

    auto pos = std::find(begin, end, ':');
    if (pos != end) {
      auto wpos = isspace(*(pos + 1)) ? pos + 2 : pos + 1; // whitespace after ':'
      headers[std::string(begin, std::distance(begin,pos))] =\
	std::string(wpos, std::distance(wpos,end));
    }
  }

  int makeStatus(const std::string& statusStr) {

    // find whitespaces around status 'HTTP/1.1 200 OK'
    size_t rb, re = 0;
    rb = statusStr.find(" ");
    if (rb == std::string::npos)
      throw std::runtime_error("Bad status line");

    re = statusStr.find(" ", rb + 1);
    if (re == std::string::npos)
      throw std::runtime_error("Bad status line");

    return std::stoi(statusStr.substr(rb, re - rb));
  }
}

Responce::Responce(const char* responceStr)
  : m_statusLine(), m_headers(), m_body() {

  static const std::string endingPattern = "\r\n";

  // iterator initialization
  const char* first = responceStr;
  const char* last = responceStr;
  while(*last++);

  // status str:
  auto rangeEnd = std::search(first, last,
			      endingPattern.cbegin(), endingPattern.cend());
  if (rangeEnd == last)
    throw std::runtime_error("Broken responce");

  m_statusLine = std::string(first, std::distance(first, rangeEnd));

  // headers:
  auto rangeBegin = rangeEnd + endingPattern.size();
  size_t len = 0;
  do {
    rangeEnd = std::search(rangeBegin, last,
			   endingPattern.cbegin(), endingPattern.cend());

    if (rangeEnd == last)
      break;

    if ((len = std::distance(rangeBegin, rangeEnd)))
      Tools::makeEntry(m_headers, rangeBegin, rangeEnd);

    rangeBegin = rangeEnd + endingPattern.size();
  } while (len);

  // body:
  m_body = std::string(rangeBegin, std::distance(rangeBegin, last));
}

const std::string& Responce::header(const std::string& hdr) const {
  auto header_it = m_headers.find(hdr);
  if (header_it == m_headers.cend())
    throw std::runtime_error("No such header: " + hdr);
    
  return header_it->second;
}

int Responce::statusCode() const {
  return Tools::makeStatus(m_statusLine);
}

std::ostream& operator<< (std::ostream& os, const Responce& rhs) {

  os << rhs.m_statusLine << '\n';
  for (const auto& header : rhs.m_headers)
    os << header.first << ':' << header.second << '\n';

  os << rhs.m_body;
  //  rhs.body.save(os, 0, pugi::format_no_declaration);
  return os;
}
