#include <catch2/catch.hpp>

#include "inch/io.hpp"

const IO io;

const std::string exe {"inch"};
const std::string unknown_flag {"-z"};
const std::string known_flag {"-i"};
const std::string valid_option {"abc123"};


TEST_CASE("No arguments given", "[IO]")
{
  const std::vector<std::string> empty_console {exe};
  const std::map<std::string, std::string> args = io.readConsoleArguments(empty_console);

  REQUIRE(args.size() == 0);
}


TEST_CASE("Flag with no value given", "[IO]")
{
  const std::vector<std::string> console {exe, unknown_flag};
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE_THAT(args.begin()->first, Catch::Matches("HELP"));
}


TEST_CASE("Value with no flag given", "[IO]")
{
  const std::vector<std::string> console {exe, valid_option};
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE_THAT(args.begin()->first, Catch::Matches("HELP"));
}


TEST_CASE("Valid arguments", "[IO]")
{
  const std::vector<std::string> console {exe, known_flag, valid_option};
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE_THAT(args.begin()->first, Catch::Matches("-i"));
  REQUIRE_THAT(args.begin()->second, Catch::Matches("abc123"));
}


TEST_CASE("Valid arguments with an unoptioned flag", "[IO]")
{
  const std::vector<std::string> console {exe, known_flag, valid_option, unknown_flag};
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE_THAT(args.begin()->first, Catch::Matches("ERROR"));
  REQUIRE_THAT(args.begin()->second, Catch::Matches("ERROR"));
}
