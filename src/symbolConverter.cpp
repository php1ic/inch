#include "symbolConverter.hpp"

std::map<std::string, int> SymbolConverter::symbolZmap = {
  {"n",0},
  {"H",1},
  {"He",2},
  {"Li",3},
  {"Be",4},
  {"B",5},
  {"C",6},
  {"N",7},
  {"O",8},
  {"F",9},
  {"Ne",10},
  {"Na",11},
  {"Mg",12},
  {"Al",13},
  {"Si",14},
  {"P",15},
  {"S",16},
  {"Cl",17},
  {"Ar",18},
  {"K",19},
  {"Ca",20},
  {"Sc",21},
  {"Ti",22},
  {"V",23},
  {"Cr",24},
  {"Mn",25},
  {"Fe",26},
  {"Co",27},
  {"Ni",28},
  {"Cu",29},
  {"Zn",30},
  {"Ga",31},
  {"Ge",32},
  {"As",33},
  {"Se",34},
  {"Br",35},
  {"Kr",36},
  {"Rb",37},
  {"Sr",38},
  {"Y",39},
  {"Zr",40},
  {"Nb",41},
  {"Mo",42},
  {"Tc",43},
  {"Ru",44},
  {"Rh",45},
  {"Pd",46},
  {"Ag",47},
  {"Cd",48},
  {"In",49},
  {"Sn",50},
  {"Sb",51},
  {"Te",52},
  {"I",53},
  {"Xe",54},
  {"Cs",55},
  {"Ba",56},
  {"La",57},
  {"Ce",58},
  {"Pr",59},
  {"Nd",60},
  {"Pm",61},
  {"Sm",62},
  {"Eu",63},
  {"Gd",64},
  {"Tb",65},
  {"Dy",66},
  {"Ho",67},
  {"Er",68},
  {"Tm",69},
  {"Yb",70},
  {"Lu",71},
  {"Hf",72},
  {"Ta",73},
  {"W",74},
  {"Re",75},
  {"Os",76},
  {"Ir",77},
  {"Pt",78},
  {"Au",79},
  {"Hg",80},
  {"Tl",81},
  {"Pb",82},
  {"Bi",83},
  {"Po",84},
  {"At",85},
  {"Rn",86},
  {"Fr",87},
  {"Ra",88},
  {"Ac",89},
  {"Th",90},
  {"Pa",91},
  {"U",92},
  {"Np",93},
  {"Pu",94},
  {"Am",95},
  {"Cm",96},
  {"Bk",97},
  {"Cf",98},
  {"Es",99},
  {"Fm",100},
  {"Md",101},
  {"No",102},
  {"Lr",103},
  {"Rf",104},
  {"Db",105},
  {"Sg",106},
  {"Bh",107},
  {"Hs",108},
  {"Mt",109},
  {"Ds",110},
  {"Rg",111},
  {"Cn",112},
  {"Ed",113},
  {"Fl",114},
  {"Ef",115},
  {"Lv",116},
  {"Eh",117},
  {"Ei",118}
};


std::string SymbolConverter::convertZToSymbol(const int Z) const
{
  auto it = std::find_if(std::begin(symbolZmap), std::end(symbolZmap),
                         [Z](const std::pair<std::string, int> element)
                         {
                           return element.second == Z;
                         }
                         );

  std::string symbol;
  if ( it == symbolZmap.end() )
    {
      std::cout << "\n**WARNING**: " << Z << "is not a valid proton number\n";
      symbol = "Xy";
    }
  else
    {
      symbol = it->first;
    }

  return symbol;
}


int SymbolConverter::convertSymbolToZ(std::string _symbol) const
{
  std::string symbol = caseCorrection(std::move(_symbol));

  auto it = std::find_if(std::begin(symbolZmap), std::end(symbolZmap),
                         [symbol](const std::pair<std::string, int> element)
                         {
                           return element.first == symbol;
                         }
                         );

  int Z;
  if ( it == symbolZmap.end() )
    {
      std::cout << "\n**WARNING**: " << symbol << "is not a valid symbol\n";
      Z = 200;
    }
  else
    {
      Z = it->second;
    }

  return Z;
}


//Allow the user to provide any case format for the symbol
//i.e, "He", "HE" and "he" will all be read as helium
std::string SymbolConverter::caseCorrection(std::string symbol) const
{
  //The only ambiguity will be neutron (n) and nitrogen (N)
  //Lets assume the user knows what they want and make no alteration
  //if n or N are provided
  if ( symbol == "n" || symbol == "N" )
    {
      return symbol;
    }

  std::transform(std::begin(symbol), std::end(symbol), std::begin(symbol), ::tolower);

  toupper(symbol.front());

  return symbol;
}
