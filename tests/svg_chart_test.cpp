#include "inch/svg_chart.hpp"

#include <catch2/catch.hpp>


TEST_CASE("SVG setup", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  const std::string setup{ "<g transform=\"translate(2.0,2.0) scale(4,4)\">\n" };

  REQUIRE_THAT(theChart.setup(), Catch::Equals(setup));
}


TEST_CASE("SVG teardown", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);
  theChart.size   = 2;
  theChart.height = 3.0;
  theChart.width  = 4.0;

  const std::string teardown{ "</g>\n"
                              "</svg>\n" };

  REQUIRE_THAT(theChart.teardown(), Catch::Equals(teardown));
}


TEST_CASE("SVG definitions", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  const std::string definitions = fmt::format(
      "<defs>\n"
      "<clipPath id=\"box\"> <rect id=\"curve\" class=\"Outline\" rx=\"0.25\" ry=\"0.25\" width=\"1\" height=\"1\" "
      "style=\"fill:none\"/> </clipPath>\n\n"
      "<g id=\"redNucleus\"> <use xlink:href=\"#curve\" class=\"Red Clip\"/> </g>\n"
      "<g id=\"greenNucleus\"> <use xlink:href=\"#curve\" class=\"Green Clip\"/> </g>\n"
      "<g id=\"blueNucleus\"> <use xlink:href=\"#curve\" class=\"Blue Clip\"/> </g>\n"
      "<g id=\"yellowNucleus\"> <use xlink:href=\"#curve\" class=\"Yellow Clip\"/> </g>\n"
      "<g id=\"cyanNucleus\"> <use xlink:href=\"#curve\" class=\"Cyan Clip\"/> </g>\n"
      "<g id=\"magentaNucleus\"> <use xlink:href=\"#curve\" class=\"Magenta Clip\"/> </g>\n"
      "<g id=\"orangeNucleus\"> <use xlink:href=\"#curve\" class=\"Orange Clip\"/> </g>\n"
      "<g id=\"whiteNucleus\"> <use xlink:href=\"#curve\" class=\"White Clip\"/> </g>\n"
      "<g id=\"blackNucleus\"> <use xlink:href=\"#curve\" class=\"Black Clip\"/> </g>\n"
      "<g id=\"navyblueNucleus\"> <use xlink:href=\"#curve\" class=\"Navy Clip\"/> </g>\n"
      "<g id=\"darkgreenNucleus\"> <use xlink:href=\"#curve\" class=\"DarkGreen Clip\"/> </g>\n"
      "<g id=\"purpleNucleus\"> <use xlink:href=\"#curve\" class=\"Purple Clip\"/> </g>\n\n"
      "<g id=\"TopHorizontalHalf\"> <path class=\"Black Clip\" d=\"M 0 0 h 1 v 0.5 h -1 Z\"/> </g>\n"
      "<g id=\"BottomHorizontalHalf\"> <path class=\"Black Clip\" d=\"M 0 1 h 1 v -0.5 h -1 Z\"/> </g>\n"
      "<g id=\"LeftVerticalHalf\"> <path class=\"Black Clip\" d=\"M0 0 h 0.5 v 1 h -0.5 Z\"/> </g>\n"
      "<g id=\"RightVerticalHalf\"> <path class=\"Black Clip\" d=\"M 0.5 0 h 0.5 v 1 h -0.5 Z\"/> </g>\n\n"
      "<g id=\"TopLeftWedge\"> <path class=\"Black Clip\" d=\"M 1 0 h -1 v 1 Z\"/> </g>\n"
      "<g id=\"TopRightWedge\"> <path class=\"Black Clip\" d=\"M 0 0 h 1 v 1 Z\"/> </g>\n"
      "<g id=\"BottomLeftWedge\"> <path class=\"Black Clip\" d=\"M 0 0 v 1 h 1 Z\"/> </g>\n"
      "<g id=\"BottomRightWedge\"> <path class=\"Black Clip\" d=\"M 0 1 h 1 v -1 Z\"/> </g>\n"
      "</defs>\n\n");

  REQUIRE(theChart.definitions() == definitions);
}


TEST_CASE("SVG colours", "SVGChart")
{
  Options options;
  const SVGChart theChart(options);

  const std::string colours = fmt::format("<style type=\"text/css\" >\n"
                                          "<![CDATA[\n"
                                          ".Red {{fill:red}}\n"
                                          ".Green {{fill:green}}\n"
                                          ".Blue {{fill:blue}}\n"
                                          ".Black {{fill:black}}\n"
                                          ".White {{fill:white}}\n"
                                          ".Yellow {{fill:yellow}}\n"
                                          ".Magenta {{fill:magenta}}\n"
                                          ".Cyan {{fill:cyan}}\n"
                                          ".Pink {{fill:pink}}\n"
                                          ".Orange {{fill:orange}}\n"
                                          ".Navy {{fill:navy}}\n"
                                          ".Purple {{fill:purple}}\n"
                                          ".DarkGreen {{fill:darkgreen}}\n"
                                          ".Outline {{stroke:black; stroke-width:0.1}}\n"
                                          ".Seri {{font-weight:normal; font-family:serif; text-decoration:none}}\n"
                                          ".MidSymbol {{font-size:0.6; text-anchor:middle}}\n"
                                          ".MidNumber {{font-size:0.35; text-anchor:middle}}\n"
                                          ".Clip {{clip-path:url(#box)}}\n"
                                          "]]>\n"
                                          "</style>\n\n");

  REQUIRE(theChart.colours() == colours);
}

TEST_CASE("SVG header", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  const std::string header =
      fmt::format("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" "
                  "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\n"
                  "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
                  "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
                  "width=\"{0}px\" height=\"{1}px\"\n"
                  "viewbox=\"0 0 {0} {1}\">\n"
                  "\n"
                  "<title></title>\n"
                  "<desc></desc>\n"
                  "<rect x=\"0\" y=\"0\" width=\"{0}\" height=\"{1}\" style=\"fill:#BBFFFF\"/>\n",
                  (4 * (2 + options.limits.getNRange())),
                  (4 * (2 + options.limits.getZRange())));

  REQUIRE(theChart.header() == header);
}


TEST_CASE("TODO SVG Key setup", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  REQUIRE_THAT(theChart.KeySetup(1), Catch::Matches(""));
}


TEST_CASE("TODO SVG Key teardown", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  REQUIRE_THAT(theChart.KeyTearDown(), Catch::Matches(""));
}


TEST_CASE("TODO SVG isotope", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  REQUIRE_THAT(theChart.isotope(), Catch::Matches(""));
}


TEST_CASE("TODO SVG infoComment", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  REQUIRE_THAT(theChart.infoComment(), Catch::Matches(""));
}
