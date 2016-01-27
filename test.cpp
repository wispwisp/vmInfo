#define BOOST_TEST_MODULE Test vmInfo

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "Command/Command.hpp"
#include "Communication/Request.hpp"
#include "Communication/Result.hpp"
#include "Client/Url.hpp"

BOOST_AUTO_TEST_SUITE(Communication)

BOOST_AUTO_TEST_CASE(Request_Test)
{
  Request r("<a>a content<b>a/b content</b><c>a/c content</c></a>");

  BOOST_CHECK(r.get("a"));
  BOOST_CHECK(not r.get("b"));
  BOOST_CHECK(r.get("a/b"));
  BOOST_CHECK(r.get("a/c"));
}


BOOST_AUTO_TEST_CASE(Result_Test)
{
  Result rs;

  auto check_option = "some_option";
  auto check_value = "some_value";

  BOOST_CHECK(not rs.get(check_option));
  rs.add(check_option, check_value);

  auto checked = rs.get(check_option);
  BOOST_CHECK(checked);
  BOOST_CHECK_EQUAL(strcmp(checked, check_value), 0);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Commands)

BOOST_AUTO_TEST_CASE(Command_Test)
{
  Request request("<uri>test:///default</uri><cmd>list</cmd>");
  Result result;

  auto cmd = Command::decodeCommand(request);
  Command::processComand(result, request, cmd);

  BOOST_CHECK_EQUAL(strcmp(result.get("domain_name"),"test"), 0);
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
}

BOOST_AUTO_TEST_SUITE_END()
