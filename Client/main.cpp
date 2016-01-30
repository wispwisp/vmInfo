#include "Url.hpp"
#include "Responce.hpp"

#include <string>
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
  std::string request = "GET " + url.resource() + " HTTP/1.0\r\n";
  boost::asio::write(s, boost::asio::buffer(request));

  // read responce
  const int MAXLEN = 1024;
  char data[MAXLEN];
  s.async_read_some(boost::asio::buffer(data, MAXLEN),
		    [&data](boost::system::error_code ec,
			    std::size_t length) {
		      if (!ec) {
			try {
			  data[length] = 0;
			  Responce responce(data);
			  if (responce.statusCode() == 200)
			    std::cout << responce;
			  else
			    std::cerr << responce.statusLine() << '\n';
			} catch(std::exception& ex) {
			  std::cerr << ex.what() << '\n';
			}
		      }
		      else
			std::cerr << ec << '\n';
		    });
  io_service.run();
}
