#include "inch/options.hpp"

#include <catch2/catch.hpp>

Options opt;

TEST_CASE("Set the filetype", "[Options]")
{
  REQUIRE(opt.filetype == FileType::EPS);

  opt.setFileType("tikz");
  REQUIRE(opt.filetype == FileType::TIKZ);

  opt.setFileType("svg");
  REQUIRE(opt.filetype == FileType::SVG);
}


TEST_CASE("String/Symbol to Int", "[Options]")
{
  REQUIRE(opt.StringToInt("C") == 6);
  REQUIRE(opt.StringToInt("He") == 2);
  REQUIRE(opt.StringToInt("Ag") == 47);
}
