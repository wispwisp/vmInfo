#include "Result.hpp"

#include <cstring>

Result::Result(std::string statusLine)
  : http{ std::move(statusLine), Headers() },
    body(), currentDomain(body) {}
  
Result::Result(std::string statusLine, Headers headers)
  : http{ std::move(statusLine), std::move(headers) },
    body(), currentDomain(body) {}


void Result::addHTTPHeader(std::string name, std::string value) {
  http.headers[std::move(name)] = std::move(value);
}


bool Result::newDomain(const char* name) noexcept {

  auto nDomain = body.append_child("domain");
  if (nDomain) {
    currentDomain = nDomain;

    currentDomain.append_attribute("name")
      .set_value(name);
  }

  return nDomain;
}

bool Result::addCurrentDomainFsInfo(const char* name,
				    const char* type,
				    const char* mountpoint) noexcept {

  auto fsInfo = currentDomain.append_child("fsInfo");
  if (fsInfo) {
    fsInfo.append_child("name")
      .text().set(name);

    fsInfo.append_child("type")
      .text().set(type);

    fsInfo.append_child("mountpoint")
      .text().set(mountpoint);
  }

  return fsInfo;
}

bool Result::addCurrentDomainFsInfo(const char* name,
				    const char* type,
				    const char* mountpoint,
				    const unsigned long& c,
				    const unsigned long& a,
				    const unsigned long& p) noexcept {

  auto fsInfo = currentDomain.append_child("fsInfo");
  if (fsInfo) {
    fsInfo.append_child("name")
      .text().set(name);

    fsInfo.append_child("type")
      .text().set(type);

    fsInfo.append_child("mountpoint")
      .text().set(mountpoint);

    fsInfo.append_child("capacity")
      .text().set(std::to_string(c).c_str());

    fsInfo.append_child("allocation")
      .text().set(std::to_string(a).c_str());

    fsInfo.append_child("physical")
      .text().set(std::to_string(p).c_str());
  }

  return fsInfo;
}

bool Result::error(const char* errStr) noexcept {
  auto err = currentDomain.append_child("error")
    .text().set(errStr);
  return err;
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
  body.print(writer, 0, pugi::format_raw);

  size_t sz = writer.written_size();
  buffer[sz] = 0;

  return sz;
}


std::ostream& operator<< (std::ostream& os, const Result& rhs) {

  os << rhs.http.statusLine << '\n';
  for (const auto& header : rhs.http.headers)
    os << header.first << ':' << header.second << '\n';

  //rhs.body.save(os, 0, pugi::format_no_declaration);
  rhs.body.save(os, 0, pugi::format_raw & pugi::format_no_declaration);

  return os;
}
