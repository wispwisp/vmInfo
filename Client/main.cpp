#include "Url.hpp"
#include "Responce.hpp"

#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char** argv)  {

  if (argc < 2) {
    std::cout << "Usage: "
	      << argv[0]
	      << " [host]:[port]/uri (Example 'localhost:8001/qemu/all/list')\n";
    return -1;
  }

  // initialization
  boost::asio::io_service io_service;

  using boost::asio::ip::tcp;
  tcp::socket s(io_service);
  tcp::resolver resolver(io_service);

  // connection
  Url url(argv[1]);
  boost::asio::connect(s, resolver.resolve({url.host(), url.port()}));

  // write request
  boost::asio::write(s, boost::asio::buffer(url.resource()));

  // read responce
  const int MAXLEN = 1024;
  char data[MAXLEN];
  s.async_read_some(boost::asio::buffer(data, MAXLEN),
		    [&data](boost::system::error_code ec,
			    std::size_t length) {
		      if (!ec) {
			data[length] = 0;
			Responce responce(data);
			std::cout << responce << '\n';
		      }
		      else
			std::cerr << ec << '\n';
		    });
  io_service.run();
}
