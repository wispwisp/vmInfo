#ifndef RESULT_H
#define RESULT_H

#include <iostream>
#include <pugixml.hpp>

class Result {
  pugi::xml_document m_result;
public:
  Result() : m_result() {};
  ~Result() = default;

  Result(const Result&) = delete;
  Result& operator= (const Result&) = delete;

  friend std::ostream& operator<< (std::ostream&, const Result&);
  size_t write(char*, size_t) const noexcept;

  const char* get(const char* option) const noexcept;
  bool add(const char* option, const char* value) noexcept;
};

#endif // RESULT_H
