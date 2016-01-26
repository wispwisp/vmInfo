#include "Result.hpp"

#include <cstring>

const char* Result::get(const char* option) const noexcept {
  auto n = m_result.first_element_by_path(option);
  if(n)
    return n.text().as_string();
  else
    return nullptr;
}


bool Result::add(const char* option, const char* value) noexcept {

  auto n = m_result.append_child(option);
  if (!n)
    return false;

  return n.text().set(value);
}


struct xml_memory_writer: pugi::xml_writer
{
  char* buffer;
  size_t capacity;
  size_t result;

  xml_memory_writer(char* buffer_, size_t capacity_)
    : buffer(buffer_), capacity(capacity_), result(0) {}
  xml_memory_writer(const xml_memory_writer&) = delete;
  xml_memory_writer& operator= (const xml_memory_writer&) = delete;

  size_t written_size() const {
    return result < capacity ? result : capacity;
  }

  virtual void write(const void* data, size_t size) {
    if (result < capacity) {
      size_t chunk = (capacity - result < size) ? capacity - result : size;
      memcpy(buffer + result, data, chunk);
    }
    result += size;
  }
};

size_t Result::write(char* buffer, size_t size) const noexcept {

  xml_memory_writer writer(buffer, size - 1);
  m_result.print(writer);

  size_t sz = writer.written_size();
  buffer[sz] = 0;

  return sz;
}


std::ostream& operator<< (std::ostream& os, const Result& rhs) {
  rhs.m_result.save(os, 0, pugi::format_no_declaration);
  return os;
}
