#include "Server.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)  {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " [port]\n";
    return -1;
  }

  int port = atoi(argv[1]);
  Server s(port);
  s.run();
}
