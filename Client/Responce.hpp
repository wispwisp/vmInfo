#ifndef RESPONCE_H
#define RESPONCE_H

#include "Body.hpp"

#include <string>
#include <unordered_map>

class Responce {
  std::string m_statusLine;
  std::unordered_map<std::string, std::string> m_headers;
  Body m_body;
public:
  explicit Responce(const char*);
  explicit Responce(const std::string& responce)
    : Responce(responce.c_str()) {};

  int statusCode() const;

  const std::string& statusLine() const noexcept
  { return m_statusLine; }

  bool isHeader(const std::string& hdr) const noexcept
  { return m_headers.count(hdr); }

  const std::string& header(const std::string&) const;

  friend std::ostream& operator<< (std::ostream&, const Responce&);
};

#endif // RESPONCE_H
