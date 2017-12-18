#include "symbolConverter.h"

SymbolConverter::SymbolConverter()
{
  populateMap();
}


void SymbolConverter::populateMap()
{
  //Store symbol as all lower case
  //Avoids forcing the user to use a specific format
  symbolZmap = {
    {"n",0},
    {"h",1},
    {"he",2},
    {"li",3},
    {"be",4},
    {"b",5},
    {"c",6},
    {"n",7},
    {"o",8},
    {"f",9},
    {"ne",10},
    {"na",11},
    {"mg",12},
    {"al",13},
    {"si",14},
    {"p",15},
    {"s",16},
    {"cl",17},
    {"ar",18},
    {"k",19},
    {"ca",20},
    {"sc",21},
    {"ti",22},
    {"v",23},
    {"cr",24},
    {"mn",25},
    {"fe",26},
    {"co",27},
    {"ni",28},
    {"cu",29},
    {"zn",30},
    {"ga",31},
    {"ge",32},
    {"as",33},
    {"se",34},
    {"br",35},
    {"kr",36},
    {"rb",37},
    {"sr",38},
    {"y",39},
    {"zr",40},
    {"nb",41},
    {"mo",42},
    {"tc",43},
    {"ru",44},
    {"rh",45},
    {"pd",46},
    {"ag",47},
    {"cd",48},
    {"in",49},
    {"sn",50},
    {"sb",51},
    {"te",52},
    {"i",53},
    {"xe",54},
    {"cs",55},
    {"ba",56},
    {"la",57},
    {"ce",58},
    {"pr",59},
    {"nd",60},
    {"pm",61},
    {"sm",62},
    {"eu",63},
    {"gd",64},
    {"tb",65},
    {"dy",66},
    {"ho",67},
    {"er",68},
    {"tm",69},
    {"yb",70},
    {"lu",71},
    {"hf",72},
    {"ta",73},
    {"w",74},
    {"re",75},
    {"os",76},
    {"ir",77},
    {"pt",78},
    {"au",79},
    {"hg",80},
    {"tl",81},
    {"pb",82},
    {"bi",83},
    {"po",84},
    {"at",85},
    {"rn",86},
    {"fr",87},
    {"ra",88},
    {"ac",89},
    {"th",90},
    {"pa",91},
    {"u",92},
    {"np",93},
    {"pu",94},
    {"am",95},
    {"cm",96},
    {"bk",97},
    {"cf",98},
    {"es",99},
    {"fm",100},
    {"md",101},
    {"no",102},
    {"lr",103},
    {"rf",104},
    {"db",105},
    {"sg",106},
    {"bh",107},
    {"hs",108},
    {"mt",109},
    {"ds",110},
    {"rg",111},
    {"cn",112},
    {"ed",113},
    {"fl",114},
    {"ef",115},
    {"lv",116},
    {"eh",117},
    {"ei",118}
  };
}


std::string SymbolConverter::convertZToSymbol(const int Z) const
{
  auto it = std::find_if(symbolZmap.begin(), symbolZmap.end(),
                         [Z](const std::pair<std::string, int> element)
                         {
                           return element.second == Z;
                         }
                         );

  return it->first;
}


int SymbolConverter::convertSymbolToZ(std::string symbol) const
{
  //Allow the user to provide any format for the symbol
  std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);

  auto it = std::find_if(symbolZmap.begin(), symbolZmap.end(),
                         [symbol](const std::pair<std::string, int> element)
                         {
                           return element.first == symbol;
                         }
                         );

  return it->second;
}
