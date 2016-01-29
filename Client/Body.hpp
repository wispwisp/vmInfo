#ifndef BODY_H
#define BODY_H

#include <string>
#include <memory>
#include <iostream>
#include <pugixml.hpp>

class Body {

  struct body_impl {
    virtual ~body_impl() = default;
    virtual void print(std::ostream&) = 0;
  };
  class body_xml final : public body_impl {
    pugi::xml_document body;
  public:
    body_xml(const char*);
    ~body_xml() = default;
    void print(std::ostream& os);
  };
  class body_string final : public body_impl {
    std::string body;
  public:
    body_string(const char*);
    ~body_string() = default;
    void print(std::ostream& os);
  };

  std::shared_ptr<body_impl> m_impl;
  explicit Body(std::shared_ptr<body_impl> impl)
    : m_impl(std::move(impl)) {};

public:
  Body() : m_impl() {};
  enum class Type { XML, String, Unknown };
  static Type decodeType(const std::string&);
  static Body createBody(Type, const char*);
  friend std::ostream& operator<< (std::ostream& os, const Body& rhs);
};

#endif // BODY_H
