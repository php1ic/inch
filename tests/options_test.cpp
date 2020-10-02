#include "inch/options.hpp"

#include <catch2/catch.hpp>


TEST_CASE("Set the filetype", "[Options]")
{
  Options opt;

  REQUIRE(opt.filetype == FileType::EPS);

  opt.setFileType("tikz");
  REQUIRE(opt.filetype == FileType::TIKZ);

  opt.setFileType("svg");
  REQUIRE(opt.filetype == FileType::SVG);
}
