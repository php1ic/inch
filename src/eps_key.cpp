#include "inch/eps_key.hpp"

#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <algorithm>


std::string EPSKey::Setup() const
{
  return fmt::format("\n%-------\n"
                     "%- Key -\n"
                     "%-------\n"
                     "/ResetWidth {{/StringLength 0 def}} def\n"
                     "/TotalWidth {{dup stringwidth pop StringLength add /StringLength exch def}} def\n"
                     "/KeyWidth 0 def\n"
                     "/TestWidth {{StringLength KeyWidth gt {{/KeyWidth StringLength def}} if}} def\n\n"
                     "%-lower left corner of the key-\n");
}


std::string EPSKey::PlaceKey(const Options& draw) const
{
  std::string origin;

  if (draw.chart_selection == ChartSelection::FULL_CHART || draw.limits.getZRange() == Limits::MAX_Z)
    {
      const int index = [&]() {
        switch (draw.chart_colour)
          {
            default:
            case ChartColour::MASSEXCESSERROR:
              return 0;
            case ChartColour::REL_MASSEXCESSERROR:
              return 1;
            case ChartColour::GS_DECAYMODE:
              return 2;
            case ChartColour::GS_HALFLIFE:
              return 3;
            case ChartColour::FIRST_ISOMERENERGY:
              return 4;
          }
      }();

      origin = fmt::format("{} {} translate", fullChartKeyPosition[index].first, fullChartKeyPosition[index].second);
    }
  else
    {
      double yOffset = 0.0;

      // The value of KEY_YOFFSET is aesthetic and is the border between
      // vertically centering the key, or vertically centering the chart.
      if (draw.limits.getZRange() >= KEY_YOFFSET)
        {
          yOffset = 0.5 * ((draw.limits.getZRange() + 1.0) - height * scale);
        }

      origin = fmt::format("{} {} translate", (draw.limits.getNRange() + 2), yOffset);
    }

  return fmt::format("{}\n"
                     "{} dup scale\n",
                     origin,
                     scale);
}


std::string EPSKey::AdditionalFunctions(const ChartColour& colour) const
{
  return [&]() {
    switch (colour)
      {
        case ChartColour::REL_MASSEXCESSERROR:
        default:
          return MassExcessSetup();
          break;
        case ChartColour::GS_HALFLIFE:
          return HalLifeSetup();
          break;
      }
  }();
}


std::string EPSKey::MassExcessSetup() const
{
  return fmt::format("\n/exponent{{\n"
                     "/e1 ed\n"
                     "/e2 ed\n"
                     "1 TR e2 5 string cvs TotalWidth sh\n"
                     "0.75 TR (x) TotalWidth sh\n"
                     "1 TR (10) TotalWidth sh\n"
                     "gs\n"
                     "0.75 TR\n"
                     "0 0.4 rmoveto e1 2 string cvs TotalWidth sh\n"
                     "gr\n"
                     "}} def\n\n"
                     "/printUnit{{\n"
                     "1 TR (   < ) TotalWidth sh\n"
                     "1 S (d) TotalWidth sh\n"
                     "1 TR (m/m < ) TotalWidth sh\n"
                     "}} def\n\n");
}


std::string EPSKey::HalLifeSetup() const
{
  return fmt::format("\n/printUnit{{gs\n"
                     "1 S (t) sh\n"
                     "0.5 TR 0 -0.15 rmoveto (1/2) sh\n"
                     "gr}} def\n\n");
}


std::string EPSKey::SurroundingBox() const
{
  // Draw a dynamically sized box around the key
  return fmt::format("\n"
                     "%Draw a box around the key\n"
                     "0.1 u div sl\n"
                     "0 0 m\n"
                     "KeyWidth 3 add 0 rl\n"
                     "0 {} rl\n"
                     "KeyWidth 3 add neg 0 rl\n"
                     "closepath\n"
                     "st\n\n",
                     height);
}


void EPSKey::Write(std::ofstream& outFile, const Partition& part) const
{
  SetText(part);

  double yPos{ 0.5 };
  // Shift the text to be y centered with the box symbol
  const double text_yShift{ 0.2 };

  // Only draw the parts of the key required
  for (auto it = part.values.crbegin(); it != part.values.crend(); ++it)
    {
      if (it->draw)
        {
          fmt::print(outFile,
                     "0 {} 0.5 {} curve Nucleus\n"
                     "2.5 {} m ResetWidth\n"
                     "{}",
                     it->colour,
                     yPos,
                     (yPos + text_yShift),
                     it->keyText);

          yPos += 1.5;
        }
    }
}
