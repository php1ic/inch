#ifndef SYMBOLCONVERTER_H
#define SYMBOLCONVERTER_H

#include <vector>
#include <string>
#include <algorithm>

class SymbolConverter
{
public:
  SymbolConverter();
  ~SymbolConverter() = default;

  std::string convertZToSymbol(const int Z) const;
  int convertSymbolToZ(std::string symbol) const;

private:
  void populateMap();
  std::vector<std::pair<std::string, int>> symbolZmap;
};

#endif // SYMBOLCONVERTER_H
