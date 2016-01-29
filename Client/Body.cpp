#include "Body.hpp"

Body::Type Body::decodeType(const std::string& contentType) {
  if (contentType == "text/xml")
    return Type::XML;

  return Type::Unknown;
}

Body Body::createBody(Body::Type type, const char* requestStr) {
  switch (type) {

  case Type::XML: return Body(std::make_shared<body_xml>(requestStr));

  case Type::Unknown:
  case Type::String: return Body(std::make_shared<body_string>(requestStr));
  }

  return Body();
}

std::ostream& operator<< (std::ostream& os, const Body& rhs) {
  if (rhs.m_impl)
    rhs.m_impl->print(os);

  return os;
}


Body::body_string::body_string(const char* s) :
  body(s) {}

void Body::body_string::print(std::ostream& os) {
  os << body;
}


Body::body_xml::body_xml(const char* s) : body() {
  body.load(s);
}

void Body::body_xml::print(std::ostream& os) {
  body.print(os);
}
