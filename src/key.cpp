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
#include <tuple>


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
  // This is future proofing for when I eventually get round to creating a GUI and everythings isn't single use
  // Make sure there is nothing in the vector before we start to fill it
  textStrings.clear();
  textStrings.resize(part.values.size());

  if (draw.chart_colour == ChartColour::MASSEXCESSERROR)
    {
      // These are used more than once so use a variable
      const std::string_view delta{ "1 S (d) TotalWidth sh" };
      const std::string_view stable{ "1 TR (Stable : ) TotalWidth sh" };

      // This string, with minor modifications, is used multiple times, wrap it in a lambda for easy reuse.
      // Also avoid any copy paste and alteration bugs if it's changed in the future.
      auto mass_comparitor = [](const double number, std::string_view comparitor) {
        return fmt::format("1 TR (m {} {} keV) TotalWidth sh TestWidth", comparitor, Converter::FloatToNdp(number));
      };

      auto m_greaterthan = [&mass_comparitor](const double number) { return mass_comparitor(number, ">"); };

      auto m_lessthan = [&mass_comparitor](const double number) { return mass_comparitor(number, "<"); };


      int index{ 0 };
      textStrings.at(index) = fmt::format("{}\n"
                                          "{}\n"
                                          "{}\n",
                                          stable,
                                          delta,
                                          m_lessthan(part.values[index].value));
      ++index;

      textStrings.at(index) = fmt::format("{}\n"
                                          "{}\n"
                                          "{}\n",
                                          stable,
                                          delta,
                                          m_greaterthan(part.values[index].value));
      ++index;

      textStrings.at(index) = fmt::format("{}\n"
                                          "{}\n",
                                          delta,
                                          m_lessthan(part.values[index].value));
      ++index;

      for (auto content = std::next(textStrings.begin(), index); content != std::prev(textStrings.end(), 1); ++content)
        {
          // content->append();
          *content = fmt::format("1 TR ({} keV < ) TotalWidth sh\n"
                                 "{}\n"
                                 "{}\n",
                                 Converter::FloatToNdp(part.values[index - 1].value),
                                 delta,
                                 m_lessthan(part.values[index].value));
          ++index;
        }

      // We do NOT want the final section value, that is INF and is just a catch all.
      textStrings.at(index) = fmt::format("{}\n"
                                          "{}\n",
                                          delta,
                                          m_greaterthan(part.values[index - 1].value));
    }
  else if (draw.chart_colour == ChartColour::REL_MASSEXCESSERROR)
    {
      auto exp_print = [](const std::tuple<std::string, std::string, std::string> number) {
        return fmt::format("{} {}{}", std::get<0>(number), std::get<1>(number), std::get<2>(number));
      };

      int index{ 0 };
      textStrings.at(index) = fmt::format("1 S (d) TotalWidth sh\n"
                                          "1 TR (m/m < ) TotalWidth sh\n"
                                          "{} exponent TestWidth",
                                          exp_print(Converter::FloatToExponent(part.values[index].value)));
      ++index;

      for (auto content = std::next(textStrings.begin(), index); content != std::prev(textStrings.end(), 1); ++content)
        {
          *content = fmt::format("{} exponent printUnit {} exponent TestWidth\n",
                                 exp_print(Converter::FloatToExponent(part.values[index - 1].value)),
                                 exp_print(Converter::FloatToExponent(part.values[index].value)));

          ++index;
        }

      textStrings.at(index) = fmt::format("1 S (d) TotalWidth sh\n"
                                          "1 TR (m/m > ) TotalWidth sh\n"
                                          "{} exponent TestWidth\n",
                                          exp_print(Converter::FloatToExponent(part.values[index - 1].value)));
    }
  else if (draw.chart_colour == ChartColour::GS_DECAYMODE)
    {
      textStrings.clear();
      textStrings.emplace_back("1 TR (Stable) TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 TR (Alpha) TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 S (b) TotalWidth sh\n0.5 TR 0 0.55 rmoveto (+) TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 S (b) TotalWidth sh\n0.75 TR 0 0.55 rmoveto (-) TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 TR (Spontaneous Fission)TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 TR (n decay) TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 TR (2n decay) TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 TR (p decay) TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 TR (2p decay) TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 TR (Unknown) TotalWidth sh TestWidth\n");
      textStrings.emplace_back("1 TR (Electron Capture) TotalWidth sh TestWidth\n");
    }
  else if (draw.chart_colour == ChartColour::GS_HALFLIFE)
    {
      auto time_comparitor = [](const std::chrono::duration<double> number, std::string_view comparitor) {
        return fmt::format(
            "printUnit 1 TR (     {} {}) TotalWidth sh TestWidth", comparitor, Converter::SecondsToHuman(number));
      };

      auto t_lessthan = [&time_comparitor](const std::chrono::duration<double> number) {
        return time_comparitor(number, "<");
      };
      auto t_greaterthan = [&time_comparitor](const std::chrono::duration<double> number) {
        return time_comparitor(number, ">");
      };

      auto dual_comparison = [](const std::chrono::duration<double> low, const std::chrono::duration<double> high) {
        return fmt::format("1 TR ({} < ) TotalWidth sh printUnit\n"
                           "(     < {}) TotalWidth sh TestWidth",
                           Converter::SecondsToHuman(low),
                           Converter::SecondsToHuman(high));
      };

      int index{ 0 };
      textStrings.at(index) = fmt::format("{}\n", t_lessthan(part.halfLifeMap.at(index)));
      ++index;

      for (auto content = std::next(textStrings.begin(), index); content != std::prev(textStrings.end(), 1); ++content)
        {
          textStrings.at(index) =
              fmt::format("{}\n", dual_comparison(part.halfLifeMap.at(index - 1), part.halfLifeMap.at(index)));

          ++index;
        }

      textStrings.at(index) = fmt::format("{}\n", t_greaterthan(part.halfLifeMap.at(index - 1)));
    }
  else if (draw.chart_colour == ChartColour::FIRST_ISOMERENERGY)
    {
      auto energy_comparitor = [](const double number, std::string_view comparitor) {
        return fmt::format(
            "1 TR (E {} {}) TotalWidth sh TestWidth", comparitor, Converter::IsomerEnergyToHuman(number));
      };

      auto e_lessthan    = [&energy_comparitor](const double number) { return energy_comparitor(number, "<"); };
      auto e_greaterthan = [&energy_comparitor](const double number) { return energy_comparitor(number, ">"); };

      auto dual_energy_comparison = [](const double low, const double high) {
        return fmt::format("1 TR ({} < E < {}) TotalWidth sh TestWidth",
                           Converter::IsomerEnergyToHuman(low),
                           Converter::IsomerEnergyToHuman(high));
      };

      int index{ 0 };
      textStrings.at(index) = fmt::format("{}\n", e_lessthan(part.values[index].value));
      ++index;

      textStrings.at(index) =
          fmt::format("{}\n", dual_energy_comparison(part.values[index - 1].value, part.values[index].value));

      for (auto content = std::next(textStrings.begin(), index); content != std::prev(textStrings.end(), 2); ++content)
        {
          textStrings.at(index) =
              fmt::format("{}\n", dual_energy_comparison(part.values[index - 1].value, part.values[index].value));

          ++index;
        }

      textStrings.at(index) = fmt::format("{}\n", e_greaterthan(part.values[index - 1].value));
      ++index;

      textStrings.at(index) = "1 TR (No known isomer) TotalWidth sh TestWidth\n";
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
