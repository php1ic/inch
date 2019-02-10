#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <map>
#include <string>
#include <tuple>


class Converter
{
public:
  // Constructors
  // default
  Converter() = default;
  // copy
  Converter(const Converter&) = default;
  // move
  Converter(Converter&&) = default;

  // Assignment
  // move
  Converter& operator=(Converter&&) = default;
  // copy
  Converter& operator=(const Converter&) = default;

  // Destructor
  ~Converter() = default;

  enum class TimeInSeconds
  {
    seconds = 1,
    minutes = 60 * seconds,
    hours   = 60 * minutes,
    days    = 24 * hours,
    years   = 365 * days
  };

  std::tuple<std::string, std::string, std::string> FloatToExponent(const double in) const;
  std::string FloatToNdp(const double number, const int numDP = 1) const;
  std::string IsomerEnergyToHuman(const double in, const int numDP = 1) const;
  std::string SecondsToHuman(const double number, const int numDP = 1) const;

  std::string convertZToSymbol(const int Z) const;
  int convertSymbolToZ(std::string _symbol) const;

private:
  std::string caseCorrection(std::string symbol) const;

  static const std::map<std::string, int>& theMap();
};

#endif // CONVERTER_HPP
