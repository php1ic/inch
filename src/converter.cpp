#include "inch/converter.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <utility>
#include <vector>


const std::vector<std::pair<std::string, int>>& Converter::symbolZmap()
{
  static const std::vector<std::pair<std::string, int>> themap{
    { "n", 0 },    { "H", 1 },    { "He", 2 },   { "Li", 3 },   { "Be", 4 },   { "B", 5 },    { "C", 6 },
    { "N", 7 },    { "O", 8 },    { "F", 9 },    { "Ne", 10 },  { "Na", 11 },  { "Mg", 12 },  { "Al", 13 },
    { "Si", 14 },  { "P", 15 },   { "S", 16 },   { "Cl", 17 },  { "Ar", 18 },  { "K", 19 },   { "Ca", 20 },
    { "Sc", 21 },  { "Ti", 22 },  { "V", 23 },   { "Cr", 24 },  { "Mn", 25 },  { "Fe", 26 },  { "Co", 27 },
    { "Ni", 28 },  { "Cu", 29 },  { "Zn", 30 },  { "Ga", 31 },  { "Ge", 32 },  { "As", 33 },  { "Se", 34 },
    { "Br", 35 },  { "Kr", 36 },  { "Rb", 37 },  { "Sr", 38 },  { "Y", 39 },   { "Zr", 40 },  { "Nb", 41 },
    { "Mo", 42 },  { "Tc", 43 },  { "Ru", 44 },  { "Rh", 45 },  { "Pd", 46 },  { "Ag", 47 },  { "Cd", 48 },
    { "In", 49 },  { "Sn", 50 },  { "Sb", 51 },  { "Te", 52 },  { "I", 53 },   { "Xe", 54 },  { "Cs", 55 },
    { "Ba", 56 },  { "La", 57 },  { "Ce", 58 },  { "Pr", 59 },  { "Nd", 60 },  { "Pm", 61 },  { "Sm", 62 },
    { "Eu", 63 },  { "Gd", 64 },  { "Tb", 65 },  { "Dy", 66 },  { "Ho", 67 },  { "Er", 68 },  { "Tm", 69 },
    { "Yb", 70 },  { "Lu", 71 },  { "Hf", 72 },  { "Ta", 73 },  { "W", 74 },   { "Re", 75 },  { "Os", 76 },
    { "Ir", 77 },  { "Pt", 78 },  { "Au", 79 },  { "Hg", 80 },  { "Tl", 81 },  { "Pb", 82 },  { "Bi", 83 },
    { "Po", 84 },  { "At", 85 },  { "Rn", 86 },  { "Fr", 87 },  { "Ra", 88 },  { "Ac", 89 },  { "Th", 90 },
    { "Pa", 91 },  { "U", 92 },   { "Np", 93 },  { "Pu", 94 },  { "Am", 95 },  { "Cm", 96 },  { "Bk", 97 },
    { "Cf", 98 },  { "Es", 99 },  { "Fm", 100 }, { "Md", 101 }, { "No", 102 }, { "Lr", 103 }, { "Rf", 104 },
    { "Db", 105 }, { "Sg", 106 }, { "Bh", 107 }, { "Hs", 108 }, { "Mt", 109 }, { "Ds", 110 }, { "Rg", 111 },
    { "Cn", 112 }, { "Ed", 113 }, { "Fl", 114 }, { "Ef", 115 }, { "Lv", 116 }, { "Eh", 117 }, { "Ei", 118 }
  };

  return themap;
}

std::string Converter::ZToSymbol(const int Z)
{
  const auto it = std::find_if(symbolZmap().cbegin(),
                               symbolZmap().cend(),
                               [Z](const std::pair<std::string, int>& element) { return element.second == Z; });

  return [&]() {
    if (it == symbolZmap().end())
      {
        fmt::print("\n**WARNING**: {} is not a valid proton number\n", Z);
        return std::string{ "Xy" };
      }

    return it->first;
  }();
}


int Converter::SymbolToZ(std::string _symbol)
{
  const std::string symbol = caseCorrection(std::move(_symbol));

  const auto it =
      std::find_if(symbolZmap().cbegin(), symbolZmap().cend(), [&symbol](const std::pair<std::string, int>& element) {
        return element.first == symbol;
      });

  return [&]() {
    if (it == symbolZmap().end())
      {
        fmt::print("\n**WARNING**: {} is not a valid symbol\n", symbol);
        return 200;
      }

    return it->second;
  }();
}


// Allow the user to provide any case format for the symbol
// i.e, "He", "HE" and "he" will all be read as helium
std::string Converter::caseCorrection(std::string symbol)
{
  // The only ambiguity will be neutron (n) and nitrogen (N)
  // Lets assume the user knows what they want and make no alteration
  // if n or N are provided
  if (symbol == "n" || symbol == "N")
    {
      return symbol;
    }

  // Convert everything to lower case
  std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);

  // Capitalise the string
  std::transform(symbol.begin(), std::next(symbol.begin()), symbol.begin(), ::toupper);

  return symbol;
}


std::tuple<std::string, std::string, std::string> Converter::FloatToExponent(const double in)
{
  // Force the number to be scientific, i.e. follow the regex: -?\d*\.?\d+e[+-]?\d+
  const std::string number = fmt::format("{0:e}", in);

  const std::regex pieces_regex(R"((-?\d*\.?\d+)e([+-]?)(\d+))");
  std::smatch matches;

  // Always get 1dp from the first number
  // If it's negative take an extra char for the sign
  const int digits = (in < 0.0) ? 4 : 3;

  return (std::regex_match(number, matches, pieces_regex))
             //              coefficient(1dp)               exponent sign            exponent
             ? std::make_tuple(
                 std::string(matches[1]).substr(0, digits), std::string(matches[2]), std::string(matches[3]))
             : std::make_tuple(std::string(), std::string(), std::string());
}


std::string Converter::IsomerEnergyToHuman(const double in, const int numDP)
{
  return (in < 1000.0) ? fmt::format("{0:0.{1}f} keV", in, numDP) : fmt::format("{0:0.{1}f} MeV", in / 1000.0, numDP);
}


std::string Converter::SecondsToHuman(const double number, const int numDP)
{
  std::string value;

  auto timeDuration = std::chrono::duration<double>{ number };

  if (isPicoSeconds(number))
    {
      value = fmt::format("{0:0.{1}f} ps", picoseconds(timeDuration).count(), numDP);
    }
  else if (isNanoSeconds(number))
    {
      value = fmt::format("{0:0.{1}f} ns", nanoseconds(timeDuration).count(), numDP);
    }
  else if (isMicroSeconds(number))
    {
      value = fmt::format("{0:0.{1}f} 1 S (u) tw sh", microseconds(timeDuration).count(), numDP);
    }
  else if (isMilliSeconds(number))
    {
      value = fmt::format("{0:0.{1}f} ms", milliseconds(timeDuration).count(), numDP);
    }
  else if (isSeconds(number))
    {
      value = fmt::format("{0:0.{1}f} s", timeDuration.count(), numDP);
    }
  else if (isMinutes(number))
    {
      value = fmt::format("{0:0.{1}f} mins", minutes(timeDuration).count(), numDP);
    }
  else if (isHours(number))
    {
      value = fmt::format("{0:0.{1}f} hrs", hours(timeDuration).count(), numDP);
    }
  else if (isDays(number))
    {
      value = fmt::format("{0:0.{1}f} days", days(timeDuration).count(), numDP);
    }
  else
    {
      const double yrs = number / static_cast<double>(TimeInSeconds::years);

      if (isBillionYears(yrs))
        {
          value = fmt::format("{0:0.{1}f} Gyrs", billionyears(timeDuration).count(), numDP);
        }
      else if (isMillionYears(yrs))
        {
          value = fmt::format("{0:0.{1}f} Myrs", millionyears(timeDuration).count(), numDP);
        }
      else
        {
          value = fmt::format("{0:0.{1}f} yrs", years(timeDuration).count(), numDP);
        }
    }

  return value;
}
