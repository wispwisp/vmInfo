#ifndef COMMAND_H
#define COMMAND_H

#include "../Communication/Result.hpp"
#include "../Communication/Request.hpp"

class Command {
public:
  enum class cmdType : char { list, fsInfo, Unknown };
  static cmdType decodeCommand(const Request& request) noexcept;
  static void processComand(Result&, const Request&, cmdType) noexcept;
};

#endif // COMMAND_H
