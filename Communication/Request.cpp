#include "Request.hpp"

#include <iostream>


Request::Request(const char* clientRequest)
  : m_request()
{

  auto parseSuccess = m_request.load_string(clientRequest);
  if (! parseSuccess) {
    m_request.append_child("error")
      .text().set(parseSuccess.description());
  }
}


const char* Request::get(const char* option) const noexcept {
  auto n = m_request.first_element_by_path(option);
  if(n)
    return n.text().as_string();
  else
    return nullptr;
}


std::ostream& operator<< (std::ostream& os, const Request& rhs) {
  rhs.m_request.save(os, 0, pugi::format_no_declaration);
  return os;
}
