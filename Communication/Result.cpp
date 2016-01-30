#include "Result.hpp"

#include <cstring>

Result::Result()
  : http{ std::string(), Headers() },
    body(), currentDomain(body) {}

Result::Result(std::string statusLine)
  : http{ std::move(statusLine), Headers() },
    body(), currentDomain(body) {}
  
Result::Result(std::string statusLine, Headers headers)
  : http{ std::move(statusLine), std::move(headers) },
    body(), currentDomain(body) {}


void Result::addStatusLine(std::string sl) {
  http.statusLine = std::move(sl);
}

void Result::addHTTPHeader(std::string name, std::string value) {
  http.headers[std::move(name)] = std::move(value);
}


bool Result::newDomain(const char* name) noexcept {

  auto nDomain = body.append_child("domain");
  bool result = nDomain;
  if (nDomain) {
    currentDomain = nDomain;

    result = currentDomain.append_attribute("name")
      .set_value(name);
  }

  return result;
}

bool Result::addCurrentDomainFsInfo(const char* name,
				    const char* type,
				    const char* mountpoint) noexcept {

  auto fsInfo = currentDomain.append_child("fsInfo");
  bool result = fsInfo;
  if (fsInfo) {
    result = fsInfo.append_child("name")
      .text().set(name);

    result = fsInfo.append_child("type")
      .text().set(type);

    result = fsInfo.append_child("mountpoint")
      .text().set(mountpoint);
  }

  return result;
}

bool Result::addCurrentDomainFsInfo(const char* name,
				    const char* type,
				    const char* mountpoint,
				    const unsigned long& c,
				    const unsigned long& a,
				    const unsigned long& p) noexcept {

  auto fsInfo = currentDomain.append_child("fsInfo");
  bool result = fsInfo;
  if (fsInfo) {
    result = fsInfo.append_child("name")
      .text().set(name);

    result = fsInfo.append_child("type")
      .text().set(type);

    result = fsInfo.append_child("mountpoint")
      .text().set(mountpoint);

    result = fsInfo.append_child("capacity")
      .text().set(static_cast<long long unsigned int>(c));

    result = fsInfo.append_child("allocation")
      .text().set(static_cast<long long unsigned int>(a));

    result = fsInfo.append_child("physical")
      .text().set(static_cast<long long unsigned int>(p));
  }

  return result;
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
size_t Result::write(char* buffer, size_t size) const {

  size_t left = size;
  char* it = buffer;

  // write status line:

  // (+2 for \r\n):
  if ((http.statusLine.size() + 2) >= size) {
    std::cerr << "Buffer overflow";
    return 0;
  }

  it = std::copy(std::begin(http.statusLine), std::end(http.statusLine), it);
  *it++ = '\r';
  *it++ = '\n';
  left = buffer - it;

  // write headers:
  for (const auto& header : http.headers) {

    // +3 becouse of ':', '\n', '\r'
    if ((header.first.size() + header.second.size() + 3) >= left) {
      std::cerr << "Buffer overflow";
      return 0;
    }

    it = std::copy(std::begin(header.first), std::end(header.first), it);
    *it++ = ':';
    it = std::copy(std::begin(header.second), std::end(header.second), it);

    *it++ = '\r';
    *it++ = '\n';

    left = buffer - it;
  }

  if (left <= 2) {
    std::cerr << "Buffer overflow";
    return 0;
  }

  *it++ = '\r';
  *it++ = '\n';
  left = buffer - it;

  if (!left) {
    std::cerr << "Buffer overflow";
    return 0;
  }

  // write body
  xml_memory_writer writer(it, left - 1);
  body.print(writer, 0, pugi::format_raw);

  size_t sz = writer.written_size();
  sz += std::distance(buffer, it);
  buffer[sz] = 0;

  return sz;
}


std::ostream& operator<< (std::ostream& os, const Result& rhs) {

  os << rhs.http.statusLine << '\n';
  for (const auto& header : rhs.http.headers)
    os << header.first << ':' << header.second << '\n';

  rhs.body.save(os, 0, pugi::format_no_declaration);
  return os;
}
