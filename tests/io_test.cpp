#include "inch/io.hpp"
#include "inch/version.hpp"

#include <catch2/catch_all.hpp>
#include <fmt/format.h>


TEST_CASE("No arguments given", "[IO]")
{
  const IO io;
  const std::string exe{ "inch" };

  const std::vector<std::string> empty_console{ exe };
  const std::map<std::string, std::string> args = io.readConsoleArguments(empty_console);

  REQUIRE(args.empty());
}


TEST_CASE("Flag with no value given", "[IO]")
{
  const IO io;
  const std::string exe{ "inch" };
  const std::string unknown_flag{ "-z" };

  const std::vector<std::string> console{ exe, unknown_flag };
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE(args.begin()->first == "HELP");
  REQUIRE(args.begin()->second == "HELP");
}


TEST_CASE("Value with no flag given", "[IO]")
{
  const IO io;
  const std::string exe{ "inch" };
  const std::string valid_option{ "abc123" };

  const std::vector<std::string> console{ exe, valid_option };
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE(args.begin()->first == "HELP");
  REQUIRE(args.begin()->second == "HELP");
}


TEST_CASE("Version info is requested", "[IO]")
{
  const IO io;
  const std::string exe{ "inch" };
  const std::string valid_flag{ "--version" };

  const std::vector<std::string> console{ exe, valid_flag };
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE(args.begin()->first == "HELP");
  REQUIRE(args.begin()->second == "HELP");
}


TEST_CASE("Help text is requested", "[IO]")
{
  const IO io;
  const std::string exe{ "inch" };
  const std::string valid_flag{ "--help" };

  const std::vector<std::string> console{ exe, valid_flag };
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE(args.begin()->first == "HELP");
  REQUIRE(args.begin()->second == "HELP");
}


TEST_CASE("Valid arguments", "[IO]")
{
  const IO io;
  const std::string exe{ "inch" };
  const std::string known_flag{ "-i" };
  const std::string valid_option{ "abc123" };

  const std::vector<std::string> console{ exe, known_flag, valid_option };
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE(args.begin()->first == "-i");
  REQUIRE(args.begin()->second == "abc123");
}


TEST_CASE("Valid arguments with an unoptioned flag", "[IO]")
{
  const IO io;
  const std::string exe{ "inch" };
  const std::string known_flag{ "-i" };
  const std::string valid_option{ "abc123" };
  const std::string unknown_flag{ "-z" };

  const std::vector<std::string> console{ exe, known_flag, valid_option, unknown_flag };
  const std::map<std::string, std::string> args = io.readConsoleArguments(console);

  REQUIRE(args.size() == 1);
  REQUIRE(args.begin()->first == "ERROR");
  REQUIRE(args.begin()->second == "ERROR");
}


TEST_CASE("Version text is dated correctly", "[IO]")
{
  const IO io;

  constexpr std::string_view date = __DATE__;

  const std::string output = fmt::format("Interactive Nuclear CHart (INCH) version {}\n"
                                         "Copyright (C) {} Everyone\n"
                                         "INCH comes with ABOLUTELY NO WARRANTY.\n"
                                         "You may redistribute copies of INCH\n"
                                         "under the terms of the GNU General Public License\n"
                                         "For more information about these matters, see the file names COPYING\n\n",
                                         INCH_VERSION,
                                         date.substr(date.rfind(' ')));

  REQUIRE(io.showVersion() == output);
}


TEST_CASE("Banner is printed correctly", "[IO]")
{
  const IO io;

  const std::string output = fmt::format("\n"
                                         "         +---+---+---+---+---+---+---+\n"
                                         "         |In |Te |Ra | C |Ti | V | E |\n"
                                         "         +---+---+---+---+---+---+---+\n"
                                         "             | N | U |Cl | E |Ar |\n"
                                         "         +---+---+---+---+---+---+\n"
                                         "         | C | H |Ar | T |\n"
                                         "         +---+---+---+---v{}\n\n",
                                         INCH_VERSION);

  REQUIRE(io.showBanner() == output);
}
