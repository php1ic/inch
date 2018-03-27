#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <string>
#include <utility>
#include <iomanip>
#include <sstream>
#include <regex>
#include <tuple>

class Converter
{
public:
  //Constructors
  //default
  Converter() = default;
  //copy
  Converter(const Converter&) = default;
  //move
  Converter(Converter&&) = default;

  //Assignment
  //move
  Converter& operator=(Converter&&) = default;
  //copy
  Converter& operator=(const Converter&) = default;

  //Destructor
  ~Converter() = default;

  std::tuple<std::string, std::string, std::string> FloatToExponent(const double in) const;
  std::string FloatToNdp(const double number, const int numDP) const;
  std::string IsomerEnergyToHuman(const double in) const;
  std::string SecondsToHuman(const double in) const;
};

#endif // CONVERTER_HPP
