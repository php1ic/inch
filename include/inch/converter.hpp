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

#include <chrono>
#include <ratio>
#include <string>
#include <tuple>
#include <vector>


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

  /// A year in seconds
  constexpr static auto year = 31556952LL;

  /// Durations that allow the use of doubles
  using picoseconds  = std::chrono::duration<double, std::pico>;
  using nanoseconds  = std::chrono::duration<double, std::nano>;
  using microseconds = std::chrono::duration<double, std::micro>;
  using milliseconds = std::chrono::duration<double, std::milli>;
  using seconds      = std::chrono::duration<double, std::ratio<1, 1>>;
  using minutes      = std::chrono::duration<double, std::ratio<60, 1>>;
  using hours        = std::chrono::duration<double, std::ratio<3600, 1>>;
  using days         = std::chrono::duration<double, std::ratio<24 * 3600, 1>>;
  using years        = std::chrono::duration<double, std::ratio<year, 1>>;
  using millionyears = std::chrono::duration<double, std::ratio<1000000 * year, 1>>;
  using billionyears = std::chrono::duration<double, std::ratio<1000000000 * year, 1>>;

  /// The std::vector<std::pair<>> used to convert Z<->Symbol
  static const std::vector<std::pair<std::string, int>>& symbolZmap();

  /**
   * Convert the the part of the string <fullString> from <start> to <end>
   *
   * \param The string to extract the value from
   * \param The first character position
   * \param The final character position
   *
   * \return The given substring as an integer
   */
  [[nodiscard]] static inline int convertStringToInt(const std::string& fullString, const int start, const int end)
  {
    return std::stoi(fullString.substr(start, end - start));
  }

  /**
   * Convert the the part of the string <fullString> from <start> to <end>
   *
   * \param The string to extract the value from
   * \param The first character position
   * \param The final character position
   *
   * \return The given substring as an double
   */
  [[nodiscard]] static inline double
  convertStringToDouble(const std::string& fullString, const int start, const int end)
  {
    return std::stod(fullString.substr(start, end - start));
  }

  /**
   * Convert a floating point number into it's scientific notation in the form of 3 separate
   * std::strings. Useful for the eps file creation
   *
   * \param in The number to convert
   *
   * \return std::tuple of std:string containing Ax10^{BC} where B is either +/-, A and C are decimal numbers
   */
  [[nodiscard]] static std::tuple<std::string, std::string, std::string> FloatToExponent(const double in);

  /**
   * Convert a floating point number to a std::string with the specified number of dp
   *
   * \param number The number to print out
   * \param numDP The number of decimal points to include in the output string
   *
   * \return A std:string of the input number, truncated to the required precision
   */
  [[nodiscard]] static std::string FloatToNdp(const double number, const int numDP = 1);

  /**
   * Convert given energy into an appropriate unit, i.e. 2.3MeV rather than 2300keV
   *
   * \param in The number to convert
   * \param numDP The precision of the value output
   *
   * \return A std::string of the given number with required precision and sensible units
   */
  [[nodiscard]] static std::string IsomerEnergyToHuman(const double in, const int numDP = 1);

  /**
   * Convert a time, in units of seconds, into scientific parlance i.e. 2.3ms rather than 0.0023s
   *
   * \param number The number to convert
   * \param numDP The precision of the value output
   *
   * \return A std::string of the given number with required precision and sensible units
   */
  [[nodiscard]] static std::string SecondsToHuman(const double number, const int numDP = 1);

  /**
   * Convert proton number to elemental symbol
   *
   * \param Z The proton value to convert
   *
   * \return The symbol as a std:string
   */
  [[nodiscard]] static std::string convertZToSymbol(const int Z);

  /**
   * Convert elemental symbol to proton number
   *
   * \param _symbol The symbol to convert
   *
   * \return The proton number as an int
   */
  [[nodiscard]] static int convertSymbolToZ(std::string _symbol);

  /**
   * Make sure symbols are correctly capitalised
   *
   * \param symbol An elemental symbol
   *
   * \return The same elememtal symbol with correct capitalisation
   */
  [[nodiscard]] static std::string caseCorrection(std::string symbol);

  /**
   * Should the time be displayed as pico seconds
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as picoseconds
   * \return [false] The time should not be displayed as picoseconds
   */
  [[nodiscard]] inline static bool isPicoSeconds(const double number) { return (number < 1.0e-10); }

  /**
   * Should the time be displayed as nano seconds
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as nanoseconds
   * \return [false] The time should not be displayed as nanoseconds
   */
  [[nodiscard]] inline static bool isNanoSeconds(const double number) { return (number < 1.0e-7 && number >= 1.0e-10); }

  /**
   * Should the time be displayed as micro seconds
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as microseconds
   * \return [false] The time should not be displayed as microseconds
   */
  [[nodiscard]] inline static bool isMicroSeconds(const double number) { return (number < 1.0e-4 && number >= 1.0e-7); }

  /**
   * Should the time be displayed as mill seconds
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as millseconds
   * \return [false] The time should not be displayed as millseconds
   */
  [[nodiscard]] inline static bool isMilliSeconds(const double number) { return (number < 0.1 && number >= 1.0e-4); }

  /**
   * Should the time be displayed as seconds
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as seconds
   * \return [false] The time should not be displayed as seconds
   */
  [[nodiscard]] inline static bool isSeconds(const double number)
  {
    return (number < static_cast<double>(TimeInSeconds::minutes) && number >= 0.1);
  }

  /**
   * Should the time be displayed as minutes
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as minutes
   * \return [false] The time should not be displayed as minutes
   */
  [[nodiscard]] inline static bool isMinutes(const double number)
  {
    return (number < static_cast<double>(TimeInSeconds::hours)
            && number >= static_cast<double>(TimeInSeconds::minutes));
  }

  /**
   * Should the time be displayed as hours
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as hours
   * \return [false] The time should not be displayed as hours
   */
  [[nodiscard]] inline static bool isHours(const double number)
  {
    return (number < static_cast<double>(TimeInSeconds::days) && number >= static_cast<double>(TimeInSeconds::hours));
  }

  /**
   * Should the time be displayed as days
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as days
   * \return [false] The time should not be displayed as days
   */
  [[nodiscard]] inline static bool isDays(const double number)
  {
    return (number < static_cast<double>(TimeInSeconds::years) && number >= static_cast<double>(TimeInSeconds::days));
  }

  /**
   * Should the time be displayed as millions years
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as millions years
   * \return [false] The time should not be displayed as millions years
   */
  [[nodiscard]] inline static bool isMillionYears(const double number) { return (number >= 1.0e6); }

  /**
   * Should the time be displayed as billions of years
   *
   * Ranges in conditions come from wanting eg 1ms rather than 1000us but still allowing eg 0.5ms
   *
   * \param The time in seconds
   *
   * \return [true] The time should be displayed as billions of years
   * \return [false] The time should not be displayed as billions of years
   */
  [[nodiscard]] inline static bool isBillionYears(const double number) { return (number >= 1.0e9); }
};

#endif // CONVERTER_HPP
