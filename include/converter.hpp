/**
 *
 * \class Converter
 *
 * \brief Utility class for the conversion of various units and types
 *
 * In the data files, values can be given to >5 dp, half lives are given
 * in scientific notation, energies are in keV, etc. This class exists
 * to convert all of these things into nice(r) formats to display in the file
 * that is output.
 *
 */
#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <map>
#include <string>
#include <tuple>


class Converter
{
public:
  Converter() = default;

  Converter(const Converter&) = default;
  Converter(Converter&&)      = default;

  Converter& operator=(Converter&&) = default;
  Converter& operator=(const Converter&) = default;

  ~Converter() = default;

  /**
   * \enum TimeInSeconds
   *
   * \brief Easy switching between seconds and {mins,hours, days,...}
   *
   * We may be able to deprecate this enum though use of [std::chrono](https://en.cppreference.com/w/cpp/chrono)
   */
  enum class TimeInSeconds
  {
    seconds = 1,
    minutes = 60 * seconds,
    hours   = 60 * minutes,
    days    = 24 * hours,
    years   = 365 * days
  };

  /**
   * Convert a floating point number into it's scientific notation in the form of 3 separate
   * std::strings. Useful for the eps file creation
   *
   * \param in The number to convert
   *
   * \return std::tuple of std:string containing Ax10^{BC} where B is either +/-, A and C are decimal numbers
   */
  std::tuple<std::string, std::string, std::string> FloatToExponent(const double in) const;

  /**
   * Convert a floating point number to a std::string with the specified number of dp
   *
   * \param number The number to print out
   * \param numDP The number of decimal points to include in the output string
   *
   * \return A std:string of the input number, truncated to the required precision
   */
  std::string FloatToNdp(const double number, const int numDP = 1) const;

  /**
   * Convert given energy into an appropriate unit, i.e. 2.3MeV rather than 2300keV
   *
   * \param in The number to convert
   * \param numDP The precision of the value output
   *
   * \return A std::string of the given number with required precision and sensible units
   */
  std::string IsomerEnergyToHuman(const double in, const int numDP = 1) const;

  /**
   * Convert a time, in units of seconds, into scientific parlance i.e. 2.3ms rather than 0.0023s
   *
   * \param number The number to convert
   * \param numDP The precision of the value output
   *
   * \return A std::string of the given number with required precision and sensible units
   */
  std::string SecondsToHuman(const double number, const int numDP = 1) const;

  /**
   * Convert proton number to elemental symbol
   *
   * \param Z The proton value to convert
   *
   * \return The symbol as a std:string
   */
  std::string convertZToSymbol(const int Z) const;

  /**
   * Convert elemental symbol to proton number
   *
   * \param _symbol The symbol to convert
   *
   * \return The proton number as an int
   */
  int convertSymbolToZ(std::string _symbol) const;

private:
  /**
   * Make sure symbols are correctly capitalised
   *
   * \param symbol An elemental symbol
   *
   * \return The same elememtal symbol with correct capitalisation
   */
  std::string caseCorrection(std::string symbol) const;

  /// The std::map used to convert Z<->Symbol
  static const std::map<std::string, int>& theMap();
};

#endif // CONVERTER_HPP
