#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <iostream>

class Request {
  bool isValid;
  std::string metod;
  std::string hypervisor;
  std::string domain;
  std::string command;
  std::string httpVersion;
public:
  explicit Request(const char*);
  friend std::ostream& operator<< (std::ostream&, const Request&);

  bool valid() const noexcept { return isValid; }
  const std::string& getMetod() const noexcept { return metod; }
  const std::string& getHypervisor() const noexcept { return hypervisor; }
  const std::string& getDomain() const noexcept { return domain; }
  const std::string& getCommand() const noexcept { return command; }
  const std::string& getVersion() const noexcept { return httpVersion; }
};

#endif // REQUEST_H
