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

#include <fmt/format.h>

#include <chrono>
#include <ratio>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

class Converter
{
public:
  Converter() = default;

  Converter(const Converter&) = default;
  Converter(Converter&&)      = default;

  Converter& operator=(Converter&&)      = default;
  Converter& operator=(const Converter&) = default;

  ~Converter() = default;

  /// A year in seconds
  constexpr static auto year = 31556952LL;

  /// Durations that allow the use of doubles rather than ints
  using attoseconds  = std::chrono::duration<double, std::atto>;
  using femtoseconds = std::chrono::duration<double, std::femto>;
  using picoseconds  = std::chrono::duration<double, std::pico>;
  using nanoseconds  = std::chrono::duration<double, std::nano>;
  using microseconds = std::chrono::duration<double, std::micro>;
  using milliseconds = std::chrono::duration<double, std::milli>;
  using seconds      = std::chrono::duration<double, std::ratio<1, 1>>;
  using minutes      = std::chrono::duration<double, std::ratio<60, 1>>;
  using hours        = std::chrono::duration<double, std::ratio<3600, 1>>;
  using days         = std::chrono::duration<double, std::ratio<static_cast<intmax_t>(24 * 3600), 1>>;
  using years        = std::chrono::duration<double, std::ratio<year, 1>>;
  using kiloyears    = std::chrono::duration<double, std::ratio<1000 * year, 1>>;
  using millionyears = std::chrono::duration<double, std::ratio<1000000 * year, 1>>;
  using billionyears = std::chrono::duration<double, std::ratio<1000000000 * year, 1>>;

  /// The std::vector<std::pair<>> used to convert Z<->Symbol
  static inline const std::vector<std::pair<std::string, int>> symbolZmap{
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

  /**
   * Convert the entire string <var> into it's integer value.
   * If the string is not an int, pass it through Converter::SymbolToZ
   *
   * \param The string to be converted
   *
   * \return[success] The string as a number
   * \return[failure] 200 signifies failure
   */
  [[nodiscard]] static int StringToInt(const std::string& var);

  /**
   * Convert the part of the string <fullString> from <start> to <end>
   *
   * \param The string to extract the value from
   * \param The first character position
   * \param The final character position
   *
   * \return The given substring as an integer
   */
  [[nodiscard]] static inline int StringToInt(const std::string& fullString, const int start, const int end)
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
  [[nodiscard]] static inline double StringToDouble(const std::string& fullString, const int start, const int end)
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
  [[nodiscard]] static std::tuple<std::string, std::string, std::string> FloatToExponent(const double value);

  /**
   * Convert a floating point number to a std::string with the specified number of dp
   *
   * \param number The number to print out
   * \param numDP The number of decimal points to include in the output string
   *
   * \return A std:string of the input number, truncated to the required precision
   */
  [[nodiscard]] static std::string FloatToNdp(const double number, const int numDP = 1)
  {
    return fmt::format("{:.{}f}", number, numDP);
  }

  /**
   * Convert given energy into an appropriate unit, i.e. 2.3MeV rather than 2300keV
   *
   * \param in The number to convert
   * \param numDP The precision of the value output
   *
   * \return A std::string of the given number with required precision and sensible units
   */
  [[nodiscard]] static std::string IsomerEnergyToHuman(const double number, const int numDP = 1);

  /**
   * Convert a time, in units of seconds, into scientific parlance i.e. 2.3ms rather than 0.0023s
   *
   * \param number The number to convert
   * \param numDP The precision of the value output
   *
   * \return A std::string of the given number with required precision and sensible units
   */
  [[nodiscard]] static std::string SecondsToHuman(const double number, const int numDP = 1)
  {
    return SecondsToHuman(seconds{ number }, numDP);
  }

  [[nodiscard]] static std::string SecondsToHuman(const std::chrono::duration<double> timeDuration,
                                                  const int numDP = 1);

  /**
   * Convert proton number to elemental symbol
   *
   * \param Z The proton value to convert
   *
   * \return The symbol as a std:string
   */
  [[nodiscard]] static std::string ZToSymbol(const int proton_number);

  /**
   * Convert elemental symbol to proton number
   *
   * \param _symbol The symbol to convert
   *
   * \return The proton number as an int
   */
  [[nodiscard]] static int SymbolToZ(std::string _symbol);

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
  [[nodiscard]] inline static bool isPicoSeconds(const std::chrono::duration<double> number)
  {
    return (number < picoseconds{ 100.0 });
  }

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
  [[nodiscard]] inline static bool isNanoSeconds(const std::chrono::duration<double> number)
  {
    return (number < nanoseconds{ 100.0 } && number >= picoseconds{ 100.0 });
  }

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
  [[nodiscard]] inline static bool isMicroSeconds(const std::chrono::duration<double> number)
  {
    return (number < microseconds{ 100.0 } && number >= nanoseconds{ 100.0 });
  }

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
  [[nodiscard]] inline static bool isMilliSeconds(const std::chrono::duration<double> number)
  {
    return (number < seconds{ 0.1 } && number >= microseconds{ 100.0 });
  }

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
  [[nodiscard]] inline static bool isSeconds(const std::chrono::duration<double> number)
  {
    return (number < minutes{ 1.0 } && number >= seconds{ 0.1 });
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
  [[nodiscard]] inline static bool isMinutes(const std::chrono::duration<double> number)
  {
    return (number < hours{ 1.0 } && number >= minutes{ 1.0 });
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
  [[nodiscard]] inline static bool isHours(const std::chrono::duration<double> number)
  {
    return (number < days{ 1.0 } && number >= hours{ 1.0 });
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
  [[nodiscard]] inline static bool isDays(const std::chrono::duration<double> number)
  {
    return (number < years{ 1.0 } && number >= days{ 1.0 });
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
  [[nodiscard]] inline static bool isMillionYears(const std::chrono::duration<double> number)
  {
    return (number >= millionyears{ 1.0 });
  }

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
  [[nodiscard]] inline static bool isBillionYears(const std::chrono::duration<double> number)
  {
    return (number >= billionyears{ 1.0 });
  }
};

#endif // CONVERTER_HPP
