#include "inch/key.hpp"

#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/converter.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <algorithm>
#include <iostream>
#include <iterator>


void Key::setScale(const Options& draw, const Partition& part) const
{
  if (!draw.key)
    {
      scale = 0.0;
      return;
    }

  // Set the key height by checking how many partition types are used
  std::for_each(part.values.cbegin(), part.values.cend(), [&](const auto val) {
    height += (val.draw) ? single_partition_height : 0;
  });

  // We don't want the key to shrink below a certain size.
  scale = (draw.limits.getZRange() > KEY_YOFFSET) ? draw.limits.getZRange() / height : KEY_YOFFSET / height;

  // Nor do we want it to be larger than a certain size.
  if (scale > max_scale || draw.chart_selection == ChartSelection::FULL_CHART
      || draw.limits.getZRange() == Limits::MAX_Z)
    {
      scale = max_scale;
    }
}


std::string Key::EPSSetup() const
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


std::string Key::EPSPlaceKey(const Options& draw) const
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


std::string Key::EPSAdditionalFunctions(const ChartColour& colour) const
{
  return [&]() {
    switch (colour)
      {
        case ChartColour::REL_MASSEXCESSERROR:
        default:
          return EPSMassExcessSetup();
          break;
        case ChartColour::GS_HALFLIFE:
          return EPSHalLifeSetup();
          break;
      }
  }();
}


std::string Key::EPSMassExcessSetup() const
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


std::string Key::EPSHalLifeSetup() const
{
  return fmt::format("\n/printUnit{{gs\n"
                     "1 S (t) sh\n"
                     "0.5 TR 0 -0.15 rmoveto (1/2) sh\n"
                     "gr}} def\n\n");
}


std::string Key::EPSSurroundingBox() const
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


void Key::EPSSetText(const Options& draw, const Partition& part) const
{
  textStrings.resize(part.values.size());

  auto text = textStrings.begin();

  if (draw.chart_colour == ChartColour::MASSEXCESSERROR)
    {
      auto low  = Converter::FloatToNdp(part.values[0].value, 1);
      auto high = Converter::FloatToNdp(part.values[1].value, 1);

      *text = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      *text += low;
      *text += " keV\\)) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m > ";
      *text += low;
      *text += " keV\\)) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 S (d) TotalWidth sh\n1 TR (m < ";
      *text += low;
      *text += " keV) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (  ";
      *text += low;
      *text += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      *text += high;
      *text += " keV) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ((text - textStrings.begin()) < static_cast<int>(part.values.size() - 1))
        {
          low   = high;
          high  = Converter::FloatToNdp(part.values[index + 1].value, 1);
          *text = "1 TR (";
          *text += low;
          *text += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
          *text += high;
          *text += " keV) TotalWidth sh TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "1 S (d) TotalWidth sh\n1 TR (m > ";
      *text += high;
      *text += " keV) TotalWidth sh TestWidth\n";
    }
  else if (draw.chart_colour == ChartColour::REL_MASSEXCESSERROR)
    {
      auto low  = Converter::FloatToExponent(part.values[0].value);
      auto high = Converter::FloatToExponent(part.values[1].value);

      *text = "1 S (d) TotalWidth sh\n1 TR (m/m < ) TotalWidth sh\n";
      *text += std::get<0>(low);
      *text += " ";
      *text += std::get<1>(low);
      *text += std::get<2>(low);
      *text += " exponent TestWidth\n";
      std::advance(text, 1);

      *text = std::get<0>(low);
      *text += " ";
      *text += std::get<1>(low);
      *text += std::get<2>(low);
      *text += " exponent printUnit ";
      *text += std::get<0>(high);
      *text += " ";
      *text += std::get<1>(high);
      *text += std::get<2>(high);
      *text += " exponent TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ((text - textStrings.begin()) < static_cast<int>(part.values.size() - 1))
        {
          low  = high;
          high = Converter::FloatToExponent(part.values[index + 1].value);

          *text = std::get<0>(low);
          *text += " ";
          *text += std::get<1>(low);
          ;
          *text += std::get<2>(low);
          *text += " exponent printUnit ";
          *text += std::get<0>(high);
          *text += " ";
          *text += std::get<1>(high);
          *text += std::get<2>(high);
          *text += " exponent TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "1 S (d) TotalWidth sh\n1 TR (m/m > ) TotalWidth sh\n";
      *text += std::get<0>(high);
      *text += " ";
      *text += std::get<1>(high);
      *text += std::get<2>(high);
      *text += " exponent TestWidth\n";
    }
  else if (draw.chart_colour == ChartColour::GS_DECAYMODE)
    {
      *text = "1 TR (Stable) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Alpha) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 S (b) TotalWidth sh\n0.5 TR 0 0.55 rmoveto (+) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 S (b) TotalWidth sh\n0.75 TR 0 0.55 rmoveto (-) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Spontaneous Fission)TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (n decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (2n decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (p decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (2p decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Unknown) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Electron Capture) TotalWidth sh TestWidth\n";
    }
  else if (draw.chart_colour == ChartColour::GS_HALFLIFE)
    {
      std::string low  = Converter::SecondsToHuman(part.halfLifeMap.at(0));
      std::string high = Converter::SecondsToHuman(part.halfLifeMap.at(1));

      *text = "printUnit 1 TR (     < ";
      *text += low;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (";
      *text += low;
      *text += " < ) TotalWidth sh printUnit\n(     < ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ((text - textStrings.begin()) < static_cast<int>(part.values.size() - 1))
        {
          low  = high;
          high = Converter::SecondsToHuman(part.halfLifeMap.at(index + 1));

          *text = "1 TR (";
          *text += low;
          *text += " < ) TotalWidth sh printUnit\n(     < ";
          *text += high;
          *text += ") TotalWidth sh TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "printUnit 1 TR (     > ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
    }
  else if (draw.chart_colour == ChartColour::FIRST_ISOMERENERGY)
    {
      auto low  = Converter::IsomerEnergyToHuman(part.values[0].value);
      auto high = Converter::IsomerEnergyToHuman(part.values[1].value);

      *text = "1 TR (E < ";
      *text += low;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (";
      *text += low;
      *text += " < E < ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      int index = 2;
      while ((text - textStrings.begin()) < static_cast<int>(part.values.size() - 2))
        {
          low  = high;
          high = Converter::IsomerEnergyToHuman(part.values[index].value);

          *text = "1 TR (";
          *text += low;
          *text += " < E < ";
          *text += high;
          *text += ") TotalWidth sh TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "1 TR (E > ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (No known isomer) TotalWidth sh TestWidth\n";
    }
}


void Key::EPSWrite(std::ofstream& outFile, const Partition& part) const
{
  // Only draw the parts of the key required
  double yPos{ 0.5 };
  for (auto it = part.values.crbegin(); it != part.values.crend(); ++it)
    {
      if (it->draw)
        {
          auto jump = std::crend(part.values) - (it + 1);

          fmt::print(outFile,
                     "0 {} 0.5 {} curve Nucleus\n"
                     "2.5 {} m ResetWidth\n"
                     "{}",
                     std::next(part.values.cbegin(), jump)->colour,
                     yPos,
                     (yPos + 0.2),
                     *std::next(textStrings.cbegin(), jump));

          yPos += 1.5;
        }
    }
}
