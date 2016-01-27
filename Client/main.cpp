#include "../Communication/Request.hpp"
#include "../Communication/Result.hpp"
#include "Url.hpp"

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

const int MAXLEN = 1024;

void processData(const char* data) {
  std::cout << data;
  // TODO:
  // get host
  // get uri (hypervisor)
  // get command

  // send request
  // recive respoce buffer ...
  // .. parse it in Result

  // present result as 'df' command output
}

int main(int argc, char** argv)  {

  if (argc < 2) {
    std::cout << "Usage: "
	      << argv[0]
	      << " [host]:[port]/uri (Example 'localhost:8001/qemu/list')\n";
    return -1;
  }

  Url url(argv[1]);

  // initialization
  boost::asio::io_service io_service;

  tcp::socket s(io_service);
  tcp::resolver resolver(io_service);
  boost::asio::connect(s, resolver.resolve({url.host(), url.port()}));

  // write request
  char request[MAXLEN] = {"<uri>test:///default</uri><cmd>list</cmd>"};
  size_t reqSz = std::strlen(request);
  boost::asio::write(s, boost::asio::buffer(request, reqSz));

  // read responce
  char data[MAXLEN];
  s.async_read_some(boost::asio::buffer(data, MAXLEN),
		    [&data](boost::system::error_code ec,
			    std::size_t length) {
		      if (!ec) {
			data[length] = 0;
			processData(data);
		      }
		      else
			std::cerr <<  ec << '\n';
		    });
  io_service.run();
}
