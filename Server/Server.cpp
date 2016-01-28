#include "Server.hpp"

#include <boost/asio.hpp>
#include <memory>

#include "../Command/Command.hpp"
#include "../Communication/Request.hpp"
#include "../Communication/Result.hpp"

namespace AsioTools{
  using boost::asio::ip::tcp;

  class session
    : public std::enable_shared_from_this<session>
  {
  public:
    session(tcp::socket socket_) : socket(std::move(socket_)) {}
    void start() {
      doRead();
    }
  private:
    void doRead() {
      auto self(shared_from_this());
      socket.async_read_some(boost::asio::buffer(data, maxLength),
			     [this, self](boost::system::error_code ec,
					  std::size_t length) {
			       if (!ec) {
				 data[length] = 0;
				 Request request(data);

				 Result result("HTTP 200 OK");
				 if (request.valid()) {
				   auto cmd = Command::decodeCommand(request);
				   Command::processComand(result, request, cmd);
				 } else
				   result.error("Bad request");

				 size_t sz = result.write(data, maxLength);
				 boost::asio::write(socket, boost::asio::buffer(data, sz));
			       }
			     });
    }

    tcp::socket socket;
    enum {maxLength = 1024};
    char data[maxLength];
  };


  class server
  {
  public:
    server(boost::asio::io_service& ioService, short port)
      : acceptor(ioService, tcp::endpoint(tcp::v4(), port)),
	socket(ioService) {
      doAccept();
    }
  private:
    void doAccept() {
      acceptor.async_accept(socket,
			    [this](boost::system::error_code ec) {
			      if (!ec)
				std::make_shared<session>(std::move(socket))->start();
			      doAccept();
			    });
    }

    tcp::acceptor acceptor;
    tcp::socket socket;
  };
}


void Server::run() const noexcept {
  try {
    boost::asio::io_service ioService;
    AsioTools::server s(ioService, port);
    ioService.run();
  }
  catch(std::exception& ex) {
    std::cerr << ex.what() << '\n';
  }
}
