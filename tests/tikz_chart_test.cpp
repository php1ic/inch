#include "inch/tikz_chart.hpp"

#include <catch2/catch_all.hpp>


TEST_CASE("TikZ sestup", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  const std::string setup{ "\\begin{document}\n"
                           "\\begin{tikzpicture}\n" };

  REQUIRE(theChart.setup() == setup);
}


TEST_CASE("TikZ teardown", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);
  theChart.size   = 2;
  theChart.height = 3.0;
  theChart.width  = 4.0;

  const std::string teardown{ "\\end{tikzpicture}\n"
                              "\\end{document}\n" };

  REQUIRE(theChart.teardown() == teardown);
}


TEST_CASE("TikZ prolog", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  const auto prolog = fmt::format("\\documentclass{{article}}\n"
                                  "\\usepackage{{tikz}}\n"
                                  "\\usepackage[T1]{{fontenc}}\n"
                                  "\\usepackage[active,tightpage]{{preview}}\n"
                                  "\\PreviewEnvironment{{tikzpicture}}\n"
                                  "\n"
                                  "%Setup command to draw box and text\n"
                                  "\\newcommand{{\\nucleus}}[6][0.25]{{\n"
                                  "\\pgfmathsetmacro{{\\rc}}{{#1*sqrt(200)}}\n"
                                  "\\filldraw[draw=black,thick,fill=#2,rounded corners=\\rc] (#3,#4) rectangle +(1,1)\n"
                                  "+(0.5,0.75) node[anchor=mid,text=black] {{#5}}\n"
                                  "+(0.5,0.27) node[anchor=mid,text=black] {{\\Large #6}};\n"
                                  "}}\n");

  REQUIRE(theChart.prolog() == prolog);
}


TEST_CASE("TODO TikZ Key setup", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  REQUIRE(theChart.KeySetup(1).empty());
}


TEST_CASE("TODO TiKZ Key teardown", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  REQUIRE(theChart.KeyTearDown().empty());
}


TEST_CASE("TODO TiKZ definitions", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  REQUIRE(theChart.definitions().empty());
}


TEST_CASE("TODO TiKZ colours", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  REQUIRE(theChart.colours().empty());
}


TEST_CASE("TODO TiKZ infoComment", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  REQUIRE(theChart.infoComment().empty());
}
