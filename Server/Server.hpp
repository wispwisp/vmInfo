#ifndef SERVER_H
#define SERVER_H

class Server {
  int port;
public:
  Server(int port_) : port(port_) {};
  void run() const noexcept;
};

#endif // SERVER_H
