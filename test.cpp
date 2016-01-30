#define BOOST_TEST_MODULE Test vmInfo

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "Command/Command.hpp"
#include "Communication/Request.hpp"
#include "Communication/Result.hpp"
#include "Client/Url.hpp"

BOOST_AUTO_TEST_SUITE(Communication)

BOOST_AUTO_TEST_CASE(request)
{
  {
    Request r("GET /qemu/all/list HTTP/1.0\r\n");

    BOOST_CHECK(r.valid());
    BOOST_CHECK_EQUAL(r.getMetod(), "GET");
    BOOST_CHECK_EQUAL(r.getHypervisor(), "qemu");
    BOOST_CHECK_EQUAL(r.getDomain(), "all");
    BOOST_CHECK_EQUAL(r.getCommand(), "list");
    BOOST_CHECK_EQUAL(r.getVersion(), "HTTP/1.0");
  }

  BOOST_CHECK(not Request("GET qemu/all/list HTTP/1.0").valid());
  BOOST_CHECK(not Request("GET qemu/all/ HTTP/1.0").valid());
  BOOST_CHECK(not Request("GET all/list HTTP/1.0").valid());
  BOOST_CHECK(not Request("GET qemu//list HTTP/1.0").valid());
  BOOST_CHECK(not Request("GET /qemu/all/list").valid());
  BOOST_CHECK(not Request("/qemu/all/list HTTP/1.0").valid());
}

BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE(ClientSide)

BOOST_AUTO_TEST_CASE(UrlParser)
{
  {
    Url u("localhost:81");
    BOOST_CHECK_EQUAL(u.host(), "localhost");
    BOOST_CHECK_EQUAL(u.port(), "81");
    BOOST_CHECK_EQUAL(u.resource(), "/");
  }
  {
    Url u("localhost");
    BOOST_CHECK_EQUAL(u.host(), "localhost");
    BOOST_CHECK_EQUAL(u.port(), "80");
    BOOST_CHECK_EQUAL(u.resource(), "/");
  }
  {
    Url u("localhost/qemu/list");
    BOOST_CHECK_EQUAL(u.host(), "localhost");
    BOOST_CHECK_EQUAL(u.port(), "80");
    BOOST_CHECK_EQUAL(u.resource(), "/qemu/list");
  }
  {
    Url u("http://localhost:81/qemu/list");
    BOOST_CHECK_EQUAL(u.host(), "localhost");
    BOOST_CHECK_EQUAL(u.port(), "81");
    BOOST_CHECK_EQUAL(u.resource(), "/qemu/list");
  }

  BOOST_CHECK_THROW(Url("http://localhost:/qemu/list"), std::runtime_error);
  BOOST_CHECK_THROW(Url("htt://localhost/qemu/list"), std::runtime_error);
  BOOST_CHECK_THROW(Url("http:localhost/qemu/list"), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
