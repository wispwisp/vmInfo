#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <pugixml.hpp>

class Request {
  pugi::xml_document m_request;
public:
  Request(const char* clientRequest);
  ~Request() = default;
  Request(const Request&) = delete;
  Request& operator= (const Request&) = delete;

  friend std::ostream& operator<< (std::ostream&, const Request&);
  const char* get(const char* option) const noexcept;
};

#endif // REQUEST_H
