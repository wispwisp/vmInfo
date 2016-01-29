#ifndef RESPONCE_H
#define RESPONCE_H

#include <string>
#include <unordered_map>

class Responce {
  std::string m_statusLine;
  std::unordered_map<std::string, std::string> m_headers;
  std::string m_body;
public:
  explicit Responce(const char*);
  explicit Responce(const std::string& responce)
    : Responce(responce.c_str()) {};

  int statusCode() const;

  const std::string& statusLine() const noexcept
  { return m_statusLine; }

  const std::string& body() const noexcept
  { return m_body; }

  bool isHeader(const std::string& hdr) const noexcept
  { return m_headers.count(hdr); }

  const std::string& header(const std::string&) const;

  const std::unordered_map<std::string, std::string>& headers() const noexcept
  { return m_headers; }

  friend std::ostream& operator<< (std::ostream&, const Responce&);
};

#endif // RESPONCE_H
