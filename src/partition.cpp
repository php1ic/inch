#include "partition.hpp"

#include "converter.hpp"

#include <algorithm>
#include <iostream>


void Partition::setDefaultColours()
{
  clearData();

  if (scheme == ChartColour::MASSEXCESSERROR)
    {
      setDefaultMassExcessColours();
    }
  else if (scheme == ChartColour::REL_MASSEXCESSERROR)
    {
      setDefaultRelativeErrorColours();
    }
  else if (scheme == ChartColour::GS_DECAYMODE)
    {
      setDefaultDecayColours();
    }
  else if (scheme == ChartColour::GS_HALFLIFE)
    {
      setDefaultHalfLifeColours();
    }
  else if (scheme == ChartColour::FIRST_ISOMERENERGY)
    {
      setDefaultIsomerEnergyColours();
    }
}

void Partition::setDefaultMassExcessColours()
{
  /// Two separate colours for stable isotopes
  values.emplace_back(section{ "black", 3.0, false });
  values.emplace_back(section{ "magenta", 3.0, false });
  ///
  values.emplace_back(section{ "blue", 3.0, false });
  values.emplace_back(section{ "cyan", 10.0, false });
  values.emplace_back(section{ "green", 20.0, false });
  values.emplace_back(section{ "yellow", 50.0, false });
  values.emplace_back(section{ "orange", 100.0, false });
  values.emplace_back(section{ "red", std::numeric_limits<double>::max(), false });
}

void Partition::setDefaultRelativeErrorColours()
{
  values.emplace_back(section("black", 2.0e-05, false));
  values.emplace_back(section("blue", 1.0e-04, false));
  values.emplace_back(section("green", 5.0e-04, false));
  values.emplace_back(section("yellow", 1.0e-03, false));
  values.emplace_back(section("orange", 5.0e-03, false));
  values.emplace_back(section("red", 1000.0, false));
}

void Partition::setDefaultDecayColours()
{
  values.emplace_back(section("black", 1.0, false));
  values.emplace_back(section("yellow", 1.0, false));
  values.emplace_back(section("blue", 1.0, false));
  values.emplace_back(section("red", 1.0, false));
  values.emplace_back(section("magenta", 1.0, false));
  values.emplace_back(section("cyan", 1.0, false));
  values.emplace_back(section("darkgreen", 1.0, false));
  values.emplace_back(section("navyblue", 1.0, false));
  values.emplace_back(section("purple", 1.0, false));
  values.emplace_back(section("green", 1.0, false));
  values.emplace_back(section("orange", 1.0, false));
}

void Partition::setDefaultHalfLifeColours()
{
  /// Use units of seconds
  values.emplace_back(section("red", 0.1, false));
  values.emplace_back(section("orange", 3.0, false));
  values.emplace_back(section("yellow", 2.0 * static_cast<double>(Converter::TimeInSeconds::minutes), false));
  values.emplace_back(section("green", 1.0 * static_cast<double>(Converter::TimeInSeconds::hours), false));
  values.emplace_back(section("cyan", 1.0 * static_cast<double>(Converter::TimeInSeconds::days), false));
  values.emplace_back(section("blue", 1.0 * static_cast<double>(Converter::TimeInSeconds::years), false));
  values.emplace_back(section("magenta", 1.0e9 * static_cast<double>(Converter::TimeInSeconds::years), false));
  values.emplace_back(section{ "black", std::numeric_limits<double>::max(), false });
}


void Partition::setDefaultIsomerEnergyColours()
{
  values.emplace_back(section("red", 20.0, false));
  values.emplace_back(section("orange", 50.0, false));
  values.emplace_back(section("yellow", 100.0, false));
  values.emplace_back(section("green", 500.0, false));
  values.emplace_back(section("blue", 1000.0, false));
  values.emplace_back(section{ "magenta", std::numeric_limits<double>::max(), false });
  /// For an empty square i.e. no isomer
  /// This should always be last.
  values.emplace_back(section("white", 1.0, false));
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
      sort(std::begin(theTable), std::end(theTable), [](const auto& lhs, const auto& rhs) {
        return lhs.NUBASE_ME < rhs.NUBASE_ME;
      });

      setAutoMassExcessColours(theTable, parts);
    }
  else if (scheme == ChartColour::REL_MASSEXCESSERROR)
    {
      sort(std::begin(theTable), std::end(theTable), [](const auto& lhs, const auto& rhs) {
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
      sort(std::begin(theTable), std::end(theTable), [](const auto& lhs, const auto& rhs) { return lhs.hl < rhs.hl; });

      setAutoHalfLifeColours(theTable, parts);
    }
  else if (scheme == ChartColour::FIRST_ISOMERENERGY)
    {
      // sort(std::begin(theTable), std::end(theTable),
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
  std::cout << parts << " | " << theTable.front().NUBASE_ME << " | " << theTable.back().NUBASE_ME << std::endl;
}

void Partition::setAutoRelativeErrorColours(std::vector<Nuclide> theTable, const int parts /*=PARTS*/)
{
  std::cout << parts << " | " << theTable.front().NUBASE_dME << " | " << theTable.back().NUBASE_dME << std::endl;
}

void Partition::setAutoHalfLifeColours(std::vector<Nuclide> theTable, const int parts /*=PARTS*/)
{
  std::cout << parts << " | " << theTable.front().hl << " | " << theTable.back().hl << std::endl;
}

void Partition::setAutoIsomerEnergyColours(std::vector<Nuclide> theTable, const int parts /*=PARTS*/)
{
  std::cout << parts << " | " << theTable.front().A << " | " << theTable.back().A << std::endl;
}

/*
void Partition::resetSort(std::vector<Nuclide> &theTable)
{
  sort(std::begin(theTable), std::end(theTable),
       []( const auto& lhs, const auto& rhs )
       {
         return lhs.A < rhs.A;
       }
       );
}
*/
