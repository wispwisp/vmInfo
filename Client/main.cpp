#include "../Communication/Request.hpp"
#include "../Communication/Result.hpp"

#include <iostream>

int main(int argc, char** argv)  {

  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " [host]:[port]/uri [command]\n";
    return -1;
  }

  // get host
  // get uri (hypervisor)
  // get command
  std::string requestStr;
  Request request(requestStr.c_str());

  // send request
  // recive respoce buffer ...
  // .. parse it in Result

  Result result;

  // present result as 'df' command output
  std::cout << result;
}
