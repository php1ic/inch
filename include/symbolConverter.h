#ifndef SYMBOLCONVERTER_H
#define SYMBOLCONVERTER_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class SymbolConverter
{
public:
  SymbolConverter();
  virtual ~SymbolConverter() = default;

  std::string convertZToSymbol(const int Z) const;
  int convertSymbolToZ(std::string _symbol) const;

private:
  void populateMap();
  std::string caseCorrection(std::string symbol) const;
  std::vector<std::pair<std::string, int>> symbolZmap;
};

#endif // SYMBOLCONVERTER_H
