#include "inch/converter.hpp"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <regex>
#include <utility>
#include <vector>


int Converter::StringToInt(const std::string& var)
{
  int number{ 0 };

  try
    {
      number = std::stoi(var);
    }
  catch (const std::invalid_argument& ia)
    {
      number = Converter::SymbolToZ(var);
    }

  return number;
}


std::string Converter::ZToSymbol(const int proton_number)
{
  const auto match = std::find_if(symbolZmap.cbegin(), symbolZmap.cend(), [proton_number](const auto element) {
    return element.second == proton_number;
  });

  return [&]() {
    if (match == symbolZmap.end())
      {
        fmt::print("\n**WARNING**: {} is not a valid proton number\n", proton_number);
        return std::string{ "Xy" };
      }

    return match->first;
  }();
}


int Converter::SymbolToZ(std::string _symbol)
{
  const std::string symbol = caseCorrection(std::move(_symbol));

  const auto match = std::find_if(
      symbolZmap.cbegin(), symbolZmap.cend(), [&symbol](const auto element) { return element.first == symbol; });

  return [&]() {
    if (match == symbolZmap.end())
      {
        fmt::print("\n**WARNING**: {} is not a valid symbol\n", symbol);
        return 200;
      }

    return match->second;
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


std::tuple<std::string, std::string, std::string> Converter::FloatToExponent(const double value)
{
  // Force the number to be scientific, i.e. follow the regex: -?\d*\.?\d+e[+-]?\d+
  const auto number = fmt::format("{0:e}", value);

  const std::regex pieces_regex(R"((-?\d*\.?\d+)e([+-]?)(\d+))");
  std::smatch matches;

  // Always get 1dp from the first number
  // If it's negative take an extra char for the sign
  const int digits = (value < 0.0) ? 4 : 3;

  return (std::regex_match(number, matches, pieces_regex))
             //              coefficient(1dp)               exponent sign            exponent
             ? std::make_tuple(
                 std::string{ matches[1] }.substr(0, digits), std::string{ matches[2] }, std::string{ matches[3] })
             : std::make_tuple(std::string{}, std::string{}, std::string{});
}


std::string Converter::IsomerEnergyToHuman(const double number, const int numDP)
{
  return (number < 1000.0) ? fmt::format("{0:0.{1}f} keV", number, numDP)
                           : fmt::format("{0:0.{1}f} MeV", number / 1000.0, numDP);
}


std::string Converter::SecondsToHuman(const std::chrono::duration<double> timeDuration, const int numDP)
{
  std::string value;

  if (isPicoSeconds(timeDuration))
    {
      value = fmt::format("{0:0.{1}f} ps", picoseconds(timeDuration).count(), numDP);
    }
  else if (isNanoSeconds(timeDuration))
    {
      value = fmt::format("{0:0.{1}f} ns", nanoseconds(timeDuration).count(), numDP);
    }
  else if (isMicroSeconds(timeDuration))
    {
      value = fmt::format("{0:0.{1}f} 1 S (u) tw sh", microseconds(timeDuration).count(), numDP);
    }
  else if (isMilliSeconds(timeDuration))
    {
      value = fmt::format("{0:0.{1}f} ms", milliseconds(timeDuration).count(), numDP);
    }
  else if (isSeconds(timeDuration))
    {
      value = fmt::format("{0:0.{1}f} s", timeDuration.count(), numDP);
    }
  else if (isMinutes(timeDuration))
    {
      value = fmt::format("{0:0.{1}f} mins", minutes(timeDuration).count(), numDP);
    }
  else if (isHours(timeDuration))
    {
      value = fmt::format("{0:0.{1}f} hrs", hours(timeDuration).count(), numDP);
    }
  else if (isDays(timeDuration))
    {
      value = fmt::format("{0:0.{1}f} days", days(timeDuration).count(), numDP);
    }
  else
    {
      if (isBillionYears(timeDuration))
        {
          value = fmt::format("{0:0.{1}f} Gyrs", billionyears(timeDuration).count(), numDP);
        }
      else if (isMillionYears(timeDuration))
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
