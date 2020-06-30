#include "inch/partition.hpp"

#include "inch/converter.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <iostream>


void Partition::setDefaultColours()
{
  clearData();

  switch (scheme)
    {
      case ChartColour::MASSEXCESSERROR:
        setDefaultMassExcessColours();
        break;
      case ChartColour::REL_MASSEXCESSERROR:
        setDefaultRelativeErrorColours();
        break;
      case ChartColour::GS_DECAYMODE:
        setDefaultDecayColours();
        break;
      case ChartColour::GS_HALFLIFE:
        setDefaultHalfLifeColours();
        break;
      case ChartColour::FIRST_ISOMERENERGY:
        setDefaultIsomerEnergyColours();
        break;
    }
}

void Partition::setDefaultMassExcessColours()
{
  // Two separate colours for stable isotopes
  values.emplace_back(section{ "black", 3.0, false });
  values.emplace_back(section{ "magenta", 3.0, false });
  //
  values.emplace_back(section{ "blue", 3.0, false });
  values.emplace_back(section{ "cyan", 10.0, false });
  values.emplace_back(section{ "green", 20.0, false });
  values.emplace_back(section{ "yellow", 50.0, false });
  values.emplace_back(section{ "orange", 100.0, false });
  values.emplace_back(section{ "red", (std::numeric_limits<double>::max)(), false });
}

void Partition::setDefaultRelativeErrorColours()
{
  values.emplace_back(section{ "black", 2.0e-05, false });
  values.emplace_back(section{ "blue", 1.0e-04, false });
  values.emplace_back(section{ "green", 5.0e-04, false });
  values.emplace_back(section{ "yellow", 1.0e-03, false });
  values.emplace_back(section{ "orange", 5.0e-03, false });
  values.emplace_back(section{ "red", 1000.0, false });
}

void Partition::setDefaultDecayColours()
{
  values.emplace_back(section{ "black", 0.0, false });
  values.emplace_back(section{ "yellow", 1.0, false });
  values.emplace_back(section{ "blue", 2.0, false });
  values.emplace_back(section{ "red", 3.0, false });
  values.emplace_back(section{ "magenta", 4.0, false });
  values.emplace_back(section{ "cyan", 5.0, false });
  values.emplace_back(section{ "darkgreen", 6.0, false });
  values.emplace_back(section{ "navyblue", 7.0, false });
  values.emplace_back(section{ "purple", 8.0, false });
  values.emplace_back(section{ "green", 9.0, false });
  values.emplace_back(section{ "orange", 10.0, false });
}

void Partition::setDefaultHalfLifeColours()
{
  // Use units of seconds
  values.emplace_back(section{ "red", 0.0, false });
  values.emplace_back(section{ "orange", 1.0, false });
  values.emplace_back(section{ "yellow", 2.0, false });
  values.emplace_back(section{ "green", 3.0, false });
  values.emplace_back(section{ "cyan", 4.0, false });
  values.emplace_back(section{ "blue", 5.0, false });
  values.emplace_back(section{ "magenta", 6.0, false });
  values.emplace_back(section{ "black", 7.0, false });
}


void Partition::setDefaultIsomerEnergyColours()
{
  values.emplace_back(section{ "red", 20.0, false });
  values.emplace_back(section{ "orange", 50.0, false });
  values.emplace_back(section{ "yellow", 100.0, false });
  values.emplace_back(section{ "green", 500.0, false });
  values.emplace_back(section{ "blue", 1000.0, false });
  values.emplace_back(section{ "magenta", (std::numeric_limits<double>::max)(), false });
  // For an empty square i.e. no isomer
  // This should always be last.
  values.emplace_back(section{ "white", 1.0, false });
}


void Partition::setUserColours(const std::vector<std::string>& _colours, const std::vector<double>& _values)
{
  clearData();

  if (scheme == ChartColour::MASSEXCESSERROR)
    {
      setUserMassExcessColours(_colours, _values);
    }
  else if (scheme == ChartColour::REL_MASSEXCESSERROR)
    {
      setUserRelativeErrorColours(_colours, _values);
    }
  else if (scheme == ChartColour::GS_DECAYMODE)
    {
      setUserDecayColours(_colours, _values);
    }
  else if (scheme == ChartColour::GS_HALFLIFE)
    {
      setUserHalfLifeColours(_colours, _values);
    }
  else if (scheme == ChartColour::FIRST_ISOMERENERGY)
    {
      setUserIsomerEnergyColours(_colours, _values);
    }
}

void Partition::setUserMassExcessColours(const std::vector<std::string>& /*_colours*/,
                                         const std::vector<double>& /*_values*/)
{
}

void Partition::setUserRelativeErrorColours(const std::vector<std::string>& /*_colours*/,
                                            const std::vector<double>& /*_values*/)
{
}

void Partition::setUserDecayColours(const std::vector<std::string>& /*_colours*/,
                                    const std::vector<double>& /*_values*/)
{
}

void Partition::setUserHalfLifeColours(const std::vector<std::string>& /*_colours*/,
                                       const std::vector<double>& /*_values*/)
{
}

void Partition::setUserIsomerEnergyColours(const std::vector<std::string>& /*_colours*/,
                                           const std::vector<double>& /*_values*/)
{
}

void Partition::setAutoColours(std::vector<Nuclide> theTable, const int parts /*=PARTS*/)
{
  clearData();

  if (scheme == ChartColour::MASSEXCESSERROR)
    {
      sort(theTable.begin(), theTable.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.NUBASE_ME < rhs.NUBASE_ME;
      });

      setAutoMassExcessColours(theTable, parts);
    }
  else if (scheme == ChartColour::REL_MASSEXCESSERROR)
    {
      sort(theTable.begin(), theTable.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.NUBASE_dME < rhs.NUBASE_dME;
      });

      setAutoRelativeErrorColours(theTable, parts);
    }
  /* Not implemented, see header file for reason
  else if ( scheme == ChartColour::GS_DECAYMODE )
    {
      setAutoDecayColours(theTable, parts);
    }
  */
  else if (scheme == ChartColour::GS_HALFLIFE)
    {
      sort(theTable.begin(), theTable.end(), [](const auto& lhs, const auto& rhs) { return lhs.hl < rhs.hl; });

      setAutoHalfLifeColours(theTable, parts);
    }
  else if (scheme == ChartColour::FIRST_ISOMERENERGY)
    {
      // sort(theTable.begin(), theTable.end(),
      //     []( const auto& lhs, const auto& rhs )
      //     {
      //       return lhs.is_nrg < rhs.is_nrg;
      //     }
      //  );

      setAutoIsomerEnergyColours(theTable, parts);
    }
}

void Partition::setAutoMassExcessColours(std::vector<Nuclide> theTable, const int parts /*=PARTS*/)
{
  fmt::print("{} | {} | {}", parts, theTable.front().NUBASE_ME, theTable.back().NUBASE_ME);
}

void Partition::setAutoRelativeErrorColours(std::vector<Nuclide> theTable, const int parts /*=PARTS*/)
{
  fmt::print("{} | {} | {}", parts, theTable.front().NUBASE_dME, theTable.back().NUBASE_dME);
}

void Partition::setAutoHalfLifeColours(std::vector<Nuclide> theTable, const int parts /*=PARTS*/)
{
  fmt::print("{} | {} | {}", parts, theTable.front().hl.count(), theTable.back().hl.count());
}

void Partition::setAutoIsomerEnergyColours(std::vector<Nuclide> theTable, const int parts /*=PARTS*/)
{
  fmt::print("{} | {} | {}", parts, theTable.front().A, theTable.back().A);
}

/*
void Partition::resetSort(std::vector<Nuclide> &theTable)
{
  sort(theTable.begin(), theTable.end(),
       []( const auto& lhs, const auto& rhs )
       {
         return lhs.A < rhs.A;
       }
       );
}
*/

std::vector<std::string> Partition::populateEPSMassExcessKeyText() const
{
  std::vector<std::string> text(values.size());

  // These are used more than once so use a variable
  const std::string_view delta{ "1 S (d) TotalWidth sh" };
  const std::string_view stable{ "1 TR (Stable : ) TotalWidth sh" };

  // This string, with minor modifications, is used multiple times, wrap it in a lambda for easy reuse.
  // Also avoid any copy paste and alteration bugs if it's changed in the future.
  auto mass_comparitor = [](const double number, std::string_view comparitor) {
    return fmt::format("1 TR (m {} {} keV) TotalWidth sh TestWidth", comparitor, Converter::FloatToNdp(number));
  };

  auto m_greaterthan = [&mass_comparitor](const double number) { return mass_comparitor(number, ">"); };
  auto m_lessthan    = [&mass_comparitor](const double number) { return mass_comparitor(number, "<"); };

  int index{ 0 };
  text.at(index) = fmt::format("{}\n"
                               "{}\n"
                               "{}\n",
                               stable,
                               delta,
                               m_lessthan(values[index].value));
  ++index;

  text.at(index) = fmt::format("{}\n"
                               "{}\n"
                               "{}\n",
                               stable,
                               delta,
                               m_greaterthan(values[index].value));
  ++index;

  text.at(index) = fmt::format("{}\n"
                               "{}\n",
                               delta,
                               m_lessthan(values[index].value));
  ++index;

  for (auto content = std::next(text.begin(), index); content != std::prev(text.end(), 1); ++content)
    {
      // content->append();
      *content = fmt::format("1 TR ({} keV < ) TotalWidth sh\n"
                             "{}\n"
                             "{}\n",
                             Converter::FloatToNdp(values[index - 1].value),
                             delta,
                             m_lessthan(values[index].value));
      ++index;
    }

  // We do NOT want the final section value, that is INF and is just a catch all.
  text.at(index) = fmt::format("{}\n"
                               "{}\n",
                               delta,
                               m_greaterthan(values[index - 1].value));

  return text;
}

std::vector<std::string> Partition::populateEPSRelativeErrorKeyText() const
{
  std::vector<std::string> text(values.size());

  auto exp_print = [](const std::tuple<std::string, std::string, std::string> number) {
    return fmt::format("{} {}{}", std::get<0>(number), std::get<1>(number), std::get<2>(number));
  };

  int index{ 0 };
  text.at(index) = fmt::format("1 S (d) TotalWidth sh\n"
                               "1 TR (m/m < ) TotalWidth sh\n"
                               "{} exponent TestWidth",
                               exp_print(Converter::FloatToExponent(values[index].value)));
  ++index;

  for (auto content = std::next(text.begin(), index); content != std::prev(text.end(), 1); ++content)
    {
      *content = fmt::format("{} exponent printUnit {} exponent TestWidth\n",
                             exp_print(Converter::FloatToExponent(values[index - 1].value)),
                             exp_print(Converter::FloatToExponent(values[index].value)));

      ++index;
    }

  text.at(index) = fmt::format("1 S (d) TotalWidth sh\n"
                               "1 TR (m/m > ) TotalWidth sh\n"
                               "{} exponent TestWidth\n",
                               exp_print(Converter::FloatToExponent(values[index - 1].value)));


  return text;
}

std::vector<std::string> Partition::populateEPSDecayModeKeyText() const
{
  return std::vector<std::string>{ "1 TR (Stable) TotalWidth sh TestWidth\n",
                                   "1 TR (Alpha) TotalWidth sh TestWidth\n",
                                   "1 S (b) TotalWidth sh\n0.5 TR 0 0.55 rmoveto (+) TotalWidth sh TestWidth\n",
                                   "1 S (b) TotalWidth sh\n0.75 TR 0 0.55 rmoveto (-) TotalWidth sh TestWidth\n",
                                   "1 TR (Spontaneous Fission)TotalWidth sh TestWidth\n",
                                   "1 TR (n decay) TotalWidth sh TestWidth\n",
                                   "1 TR (2n decay) TotalWidth sh TestWidth\n",
                                   "1 TR (p decay) TotalWidth sh TestWidth\n",
                                   "1 TR (2p decay) TotalWidth sh TestWidth\n",
                                   "1 TR (Unknown) TotalWidth sh TestWidth\n",
                                   "1 TR (Electron Capture) TotalWidth sh TestWidth\n" };
}

std::vector<std::string> Partition::populateEPSHalfLifeKeyText() const
{
  std::vector<std::string> text(values.size());

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
  text.at(index) = fmt::format("{}\n", t_lessthan(halfLifeMap.at(index)));
  ++index;

  for (auto content = std::next(text.begin(), index); content != std::prev(text.end(), 1); ++content)
    {
      text.at(index) = fmt::format("{}\n", dual_comparison(halfLifeMap.at(index - 1), halfLifeMap.at(index)));

      ++index;
    }

  text.at(index) = fmt::format("{}\n", t_greaterthan(halfLifeMap.at(index - 1)));

  return text;
}

std::vector<std::string> Partition::populateEPSIsomerEnergyKeyText() const
{
  std::vector<std::string> text(values.size());

  auto energy_comparitor = [](const double number, std::string_view comparitor) {
    return fmt::format("1 TR (E {} {}) TotalWidth sh TestWidth", comparitor, Converter::IsomerEnergyToHuman(number));
  };

  auto e_lessthan    = [&energy_comparitor](const double number) { return energy_comparitor(number, "<"); };
  auto e_greaterthan = [&energy_comparitor](const double number) { return energy_comparitor(number, ">"); };

  auto dual_energy_comparison = [](const double low, const double high) {
    return fmt::format("1 TR ({} < E < {}) TotalWidth sh TestWidth",
                       Converter::IsomerEnergyToHuman(low),
                       Converter::IsomerEnergyToHuman(high));
  };

  int index{ 0 };
  text.at(index) = fmt::format("{}\n", e_lessthan(values[index].value));
  ++index;

  text.at(index) = fmt::format("{}\n", dual_energy_comparison(values[index - 1].value, values[index].value));

  for (auto content = std::next(text.begin(), index); content != std::prev(text.end(), 2); ++content)
    {
      text.at(index) = fmt::format("{}\n", dual_energy_comparison(values[index - 1].value, values[index].value));

      ++index;
    }

  text.at(index) = fmt::format("{}\n", e_greaterthan(values[index - 1].value));
  ++index;

  text.at(index) = "1 TR (No known isomer) TotalWidth sh TestWidth\n";


  return text;
}

std::vector<std::string> Partition::populateEPSKeyText() const
{
  return [&]() {
    switch (scheme)
      {
        case ChartColour::MASSEXCESSERROR:
        default:
          return populateEPSMassExcessKeyText();
        case ChartColour::REL_MASSEXCESSERROR:
          return populateEPSRelativeErrorKeyText();
        case ChartColour::GS_DECAYMODE:
          return populateEPSDecayModeKeyText();
        case ChartColour::GS_HALFLIFE:
          return populateEPSHalfLifeKeyText();
        case ChartColour::FIRST_ISOMERENERGY:
          return populateEPSIsomerEnergyKeyText();
      }
  }();
}
