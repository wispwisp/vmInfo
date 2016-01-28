#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <iostream>
#include <pugixml.hpp>
#include <unordered_map>

class Result {

  using Headers = std::unordered_map<std::string, std::string>;
  struct httpResponceHead {
    std::string statusLine;
    Headers headers;
  };

  httpResponceHead http;
  pugi::xml_document body;
  pugi::xml_node currentDomain;

public:
  // headers
  Result(std::string statusLine);
  Result(std::string statusLine, Headers headers);
  void addHTTPHeader(std::string, std::string);

  // body
  bool newDomain(const char* name) noexcept;
  bool addCurrentDomainFsInfo(const char*, const char*, const char*) noexcept;
  bool addCurrentDomainFsInfo(const char* name,
			      const char* type,
			      const char* mountpoint,
			      // could be undefined:
			      const unsigned long& c,
			      const unsigned long& a,
			      const unsigned long& p) noexcept;

  bool error(const char*) noexcept;

  // pugixml output:
  friend std::ostream& operator<< (std::ostream&, const Result&);
  size_t write(char*, size_t) const noexcept;
};

#endif // RESULT_H
