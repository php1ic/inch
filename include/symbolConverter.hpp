#ifndef SYMBOLCONVERTER_HPP
#define SYMBOLCONVERTER_HPP

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>

class SymbolConverter
{
public:
  //Constructors
  //default
  SymbolConverter() = default;
  //copy
  SymbolConverter(const SymbolConverter&) = default;
  //move
  SymbolConverter(SymbolConverter&&) = default;

  //Assignment
  //move
  SymbolConverter& operator=(SymbolConverter&&) = default;
  //copy
  SymbolConverter& operator=(const SymbolConverter&) = default;

  //Destructor
  virtual ~SymbolConverter() = default;

  const std::string convertZToSymbol(const int Z) const;
  int convertSymbolToZ(std::string _symbol) const;

private:
  const std::string caseCorrection(std::string symbol) const;

  static const std::map<std::string, int>& theMap();
};

#endif // SYMBOLCONVERTER_HPP
