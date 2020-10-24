#include "inch/svg_chart.hpp"

#include "inch/nuclide.hpp"
#include "inch/options.hpp"

#include <fmt/ostream.h>


void SVGChart::write(const std::vector<Nuclide>& massTable, const Partition& /*part*/) const
{
  std::ofstream outFile(options.outfile, std::ios::binary);

  if (!outFile.is_open())
    {
      fmt::print("\n***ERROR***: Couldn't open {} to create the chart.\n", options.outfile);
      return;
    }

  fmt::print(outFile, "{}", prolog());

  fmt::print(outFile, "{}", setup());

  for (const auto& isotope : massTable)
    {
      if (isotope.show == 1 || isotope.show == 2)
        {
          fmt::print(outFile, "{}", isotope.writeAsSVG(options.limits.Nmin, options.limits.Zmax));
        }
    }

  fmt::print(outFile, "{}", teardown());
}


std::string SVGChart::header() const
{
  return fmt::format("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" "
                     "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\n"
                     "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
                     "xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
                     "width=\"{0}px\" height=\"{1}px\"\n"
                     "viewbox=\"0 0 {0} {1}\">\n"
                     "\n"
                     "<title></title>\n"
                     "<desc></desc>\n"
                     "<rect x=\"0\" y=\"0\" width=\"{0}\" height=\"{1}\" style=\"fill:#BBFFFF\"/>\n",
                     (size * (2 + options.limits.getNRange())),
                     (size * (2 + options.limits.getZRange())));
}


std::string SVGChart::definitions() const
{
  return fmt::format(
      "<defs>\n"
      "<clipPath id=\"box\"> <rect id=\"curve\" class=\"Outline\" rx=\"{0}\" ry=\"{0}\" width=\"1\" height=\"1\" "
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
      "</defs>\n\n",
      curve);
}


std::string SVGChart::colours() const
{
  return fmt::format("<style type=\"text/css\" >\n"
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
}


std::string SVGChart::prolog() const
{
  return header() + definitions() + colours();
}


std::string SVGChart::setup() const
{
  return fmt::format("<g transform=\"translate({0:0.1f},{0:0.1f}) scale({1},{1})\">\n", 0.5 * size, size);
}


std::string SVGChart::teardown() const
{
  return fmt::format("</g>\n"
                     "</svg>\n");
}
