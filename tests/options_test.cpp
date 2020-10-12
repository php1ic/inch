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

  opt.setFileType("eps");
  REQUIRE(opt.filetype == FileType::EPS);
}


TEST_CASE("Output file is sanitised", "[Options]")
{
  Options options;

  SECTION("Extension is removed if given, EPS")
  {
    options.outfile  = "chart.extension";
    options.filetype = FileType::EPS;

    REQUIRE(options.validateOutputFilename());
    REQUIRE(options.outfile == "chart.eps");
  }

  SECTION("Extension is removed if given, SVG")
  {
    options.outfile  = "chart.extension";
    options.filetype = FileType::SVG;

    REQUIRE(options.validateOutputFilename());
    REQUIRE(options.outfile == "chart.svg");
  }

  SECTION("Extension is removed if given, TIKZ")
  {
    options.outfile  = "chart.extension";
    options.filetype = FileType::TIKZ;

    REQUIRE(options.validateOutputFilename());
    REQUIRE(options.outfile == "chart.tex");
  }

  SECTION("Try to output to a directory fall back to defalt name")
  {
    std::filesystem::create_directory("test_dir");
    options.outfile  = "test_dir";
    options.filetype = FileType::EPS;

    REQUIRE(options.validateOutputFilename());
    REQUIRE(options.outfile == "chart.eps");
  }
}


TEST_CASE("Don't overwrite exising file", "[Options]")
{
  Options options;

  SECTION("Unless it's called chart")
  {
    options.outfile  = "chart";
    options.filetype = FileType::EPS;

    options.setOutputFilename();
    REQUIRE(options.outfile == "chart.eps");
  }
}

TEST_CASE("Input options are valid", "[Options]")
{
  Options options;

  SECTION("No values are given")
  {
    std::map<std::string, std::string> values{};

    REQUIRE_FALSE(options.checkInputFileOptions(values));
  }

  SECTION("Not enough values are given")
  {
    std::map<std::string, std::string> values{ { "section", "1" }, { "type", "0" } };

    REQUIRE_FALSE(options.checkInputFileOptions(values));
  }
}
