#include "Body.hpp"

#include <string>
#include <cstring>

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

namespace XmlPrint {
  bool same(const char* l, const char* r) {
    return !strcmp(l,r);
  }

  void globalError(std::ostream& os, const pugi::xml_node& error) {
    os << "Error: " << error.text().as_string() << '\n';
  }

  void domainError(std::ostream& os, const pugi::xml_node& error) {
    os << "Domain error: " << error.text().as_string() << '\n';
  }

  void printFS(std::ostream& os, const pugi::xml_node& fsInfo) {
    os << "Filesystem 1K-blocks Used Available Use% Mounted on\n";

    pugi::xml_node name, type, mountpoint;
    if ((name = fsInfo.child("name")) &&
	(type = fsInfo.child("type")) &&
	(mountpoint = fsInfo.child("mountpoint"))) {

      os << name.text().as_string() << ' ';

      pugi::xml_node capacity, allocation, physical;
      if ((capacity = fsInfo.child("capacity")) &&
	  (allocation = fsInfo.child("allocation")) &&
	  (physical = fsInfo.child("physical"))) {

	auto c = std::stoll(capacity.text().as_string());
	auto a = std::stoll(allocation.text().as_string());
	// auto p = std::stoll(physical.text().as_string()); // physical on host machine

	os << c / 1024 << ' ' // size total (as guest see)
	   << (c - a) / 1024 << ' ' // aviable
	   << a / 1024 <<' ' // used
	   << (c-a) / (c / 100)
	   << "% ";// percent

      } else
	os << "Info unaviable ";

      os << type.text().as_string() << ' '
	 << mountpoint.text().as_string()
	 << '\n';
    }
  }

  void printDomain(std::ostream& os, const pugi::xml_node& domain) {

    os << "Domain '" << domain.attribute("name").value() << "':\n";

    for (const auto& node : domain.children()) {
      if (same(node.name(), "error"))
	domainError(os, node);
      else if (same(node.name(), "fsInfo"))
	printFS(os, node);
    }
  }
}

void Body::body_xml::print(std::ostream& os) {

  using namespace XmlPrint;
  for (const auto& node : body.children()) {
    if (same(node.name(), "error"))
      globalError(os, node);
    else if (same(node.name(), "domain"))
      printDomain(os, node);
  }
}
