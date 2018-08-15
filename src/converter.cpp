#include "converter.hpp"

const std::map<std::string, int>& Converter::theMap()
{
  const static std::map<std::string, int> symbolZmap = {
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

  return symbolZmap;
}

std::string Converter::convertZToSymbol(const int Z) const
{
  const auto it = std::find_if(std::cbegin(theMap()), std::cend(theMap()),
                               [Z](const std::pair<std::string, int> element)
                               {
                                 return element.second == Z;
                               }
                               );

  return [&]()
    {
      if ( it == theMap().end() )
        {
          std::cout << "\n**WARNING**: " << Z << "is not a valid proton number\n";
          return std::string {"Xy"};
        }

      return it->first;
    }();
}


int Converter::convertSymbolToZ(std::string _symbol) const
{
  const std::string symbol = caseCorrection(std::move(_symbol));

  const auto it = theMap().find(symbol);

  return[&]()
    {
      if ( it == theMap().end() )
        {
          std::cout << "\n**WARNING**: " << symbol << "is not a valid symbol\n";
          return 200;
        }

      return it->second;
    }();
}


//Allow the user to provide any case format for the symbol
//i.e, "He", "HE" and "he" will all be read as helium
const std::string Converter::caseCorrection(std::string symbol) const
{
  //The only ambiguity will be neutron (n) and nitrogen (N)
  //Lets assume the user knows what they want and make no alteration
  //if n or N are provided
  if ( symbol == "n" || symbol == "N" )
    {
      return symbol;
    }

  std::transform(std::begin(symbol), std::end(symbol), std::begin(symbol), ::tolower);

  symbol.front() = toupper(symbol.front());

  return symbol;
}


std::string Converter::FloatToNdp(const double number, const int numDP) const
{
  std::string value = std::to_string(number);

  const int digits = (numDP < 1) ? -1 : numDP+1;

  return value.substr(0, value.find('.') + digits);
}


std::tuple<std::string, std::string, std::string> Converter::FloatToExponent(const double in) const
{
  /// Force the number to follow the regex: -?\d*\.?\d+e[+-]?\d+
  std::ostringstream num;
  num << std::scientific << in;

  const std::regex pieces_regex(R"((-?\d*\.?\d+)e([+-]?)(\d+))");
  std::smatch matches;

  /// Need to convert before passing to regex_match
  const std::string number = num.str();

  /// Always get 1dp from the first number
  /// If it's negative take an extra char for the sign
  const int digits = ( in < 0.0 ) ? 4 : 3;

  return ( std::regex_match(number, matches, pieces_regex) )
    //                coefficient(1dp)                          exponent sign           exponent
    ? std::make_tuple(std::string(matches[1]).substr(0, digits),std::string(matches[2]),std::string(matches[3]))
    : std::make_tuple(std::string(), std::string(), std::string());
}


std::string Converter::IsomerEnergyToHuman(const double in, const int numDP) const
{
  return ( in < 1.0e3 )
    ? FloatToNdp(in, numDP) + " keV"
    : FloatToNdp(in/1.0e3, numDP) + " MeV";
}


std::string Converter::SecondsToHuman(const double number, const int numDP) const
{
  std::string value;

  /// Don't forget the space at the front of the units string
  /// Ranges in conditions come from wanting eg 1ms rather than 1000us
  /// but still allowing eg 0.5ms
  if ( number < 1.0e-10 )
    {
      value = FloatToNdp(number*1.0e12, numDP) + " ps";
    }
  else if ( number < 1.0e-7 && number >= 1.0e-10 )
    {
      value = FloatToNdp(number*1.0e9, numDP) + " ns";
    }
  else if ( number < 1.0e-4 && number >= 1.0e-7 )
    {
      value = FloatToNdp(number*1.0e6, numDP) + " 1 S (u) tw sh";
    }
  else if ( number < 0.1 && number >= 1.0e-4 )
    {
      value =  FloatToNdp(number*1.0e3, numDP) + " ms";
    }
  else if ( number < static_cast<double>(TimeInSeconds::minutes) && number >= 0.1 )
    {
      value = FloatToNdp(number, numDP) + " s";
    }
  else if (    number < static_cast<double>(TimeInSeconds::hours)
           && number >= static_cast<double>(TimeInSeconds::minutes) )
    {
      value = FloatToNdp(number/static_cast<double>(TimeInSeconds::minutes), numDP) + " mins";
    }
  else if (    number < static_cast<double>(TimeInSeconds::days)
           && number >= static_cast<double>(TimeInSeconds::hours) )
    {
      value = FloatToNdp(number/static_cast<double>(TimeInSeconds::hours), numDP) + " hrs";
    }
  else if (    number < static_cast<double>(TimeInSeconds::years)
           && number >= static_cast<double>(TimeInSeconds::days) )
    {
      value = FloatToNdp(number/static_cast<double>(TimeInSeconds::days), numDP) + " days";
    }
  else
    {
      const double years = number/static_cast<double>(TimeInSeconds::years);

      if ( years >= 1.0e9 )
        {
          value = FloatToNdp(years/1.0e9, numDP) + " Gyrs";
        }
      else if ( years >= 1.0e6 )
        {
          value = FloatToNdp(years/1.0e6, numDP) + " Myrs";
        }
      else
        {
          value = FloatToNdp(years, numDP) + " yrs";
        }
    }

  return value;
}
