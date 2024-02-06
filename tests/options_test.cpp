#include "inch/options.hpp"

#include <catch2/catch_all.hpp>


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

  SECTION("Invalid values are given")
  {
    std::map<std::string, std::string> values{ { "section", "1" }, { "BAD_VALUE", "0" } };

    REQUIRE_FALSE(options.checkInputFileOptions(values));
  }

  SECTION("Invalid type value")
  {
    std::map<std::string, std::string> values{ { "section", "1" }, { "type", "6" }, { "choice", "0" } };

    REQUIRE_FALSE(options.checkInputFileOptions(values));
  }

  SECTION("Invalid choice value")
  {
    std::map<std::string, std::string> values{ { "section", "0" }, { "type", "2" }, { "choice", "9" } };

    REQUIRE_FALSE(options.checkInputFileOptions(values));
  }

  SECTION("Zmin values are read and sanitized")
  {
    std::map<std::string, std::string> values{
      { "section", "0" }, { "type", "1" }, { "choice", "0" }, { "Zmin", "15" }
    };

    SECTION("Zmin is set correctly")
    {
      REQUIRE(options.checkInputFileOptions(values));
      REQUIRE(options.limits.Zmin == 15);
    }

    SECTION("Zmin is set too low")
    {
      values["Zmin"] = "-1";
      REQUIRE_FALSE(options.checkInputFileOptions(values));
    }

    SECTION("Zmin is set too high")
    {
      values["Zmin"] = "150";
      REQUIRE_FALSE(options.checkInputFileOptions(values));
    }
  }

  SECTION("Zmax values are read and sanitized")
  {
    std::map<std::string, std::string> values{
      { "section", "0" }, { "type", "2" }, { "choice", "1" }, { "Zmax", "76" }
    };

    SECTION("Zmax is set correctly")
    {
      REQUIRE(options.checkInputFileOptions(values));
      REQUIRE(options.limits.Zmax == 76);
    }

    SECTION("Zmax is set too low")
    {
      values["Zmax"] = "-1";
      REQUIRE_FALSE(options.checkInputFileOptions(values));
    }

    SECTION("Zmin is set too high")
    {
      values["Zmax"] = "150";
      REQUIRE_FALSE(options.checkInputFileOptions(values));
    }
  }

  SECTION("All neutrons required")
  {
    std::map<std::string, std::string> values{
      { "section", "0" }, { "type", "2" }, { "choice", "2" }, { "required", "0" }
    };

    REQUIRE(options.checkInputFileOptions(values));
  }

  SECTION("Nmin values are read and sanitized")
  {
    std::map<std::string, std::string> values{
      { "section", "0" }, { "type", "2" }, { "choice", "3" }, { "Nmin", "21" }
    };

    SECTION("Nmin is set correctly")
    {
      REQUIRE(options.checkInputFileOptions(values));
      REQUIRE(options.limits.Nmin == 21);
    }

    SECTION("Nmin is set too low")
    {
      values["Nmin"] = "-1";
      REQUIRE_FALSE(options.checkInputFileOptions(values));
    }

    SECTION("Nmin is set too high")
    {
      values["Nmin"] = "200";
      REQUIRE_FALSE(options.checkInputFileOptions(values));
    }
  }

  SECTION("Nmax values are read and sanitized")
  {
    std::map<std::string, std::string> values{
      { "section", "0" }, { "type", "2" }, { "choice", "4" }, { "Nmax", "89" }
    };

    SECTION("Nmax is set correctly")
    {
      REQUIRE(options.checkInputFileOptions(values));
      REQUIRE(options.limits.Nmax == 89);
    }

    SECTION("Nmax is set too low")
    {
      values["Nmax"] = "-1";
      REQUIRE_FALSE(options.checkInputFileOptions(values));
    }

    SECTION("Nmin is set too high")
    {
      values["Nmax"] = "200";
      REQUIRE_FALSE(options.checkInputFileOptions(values));
    }
  }
}
