#ifndef NUCLIDE_HPP
#define NUCLIDE_HPP

#include <cmath>
#include <sstream>
#include <string>
#include <vector>


class Nuclide
{
public:
  // Constructors
  // default
  explicit Nuclide(std::string line) : full_data(std::move(line)) {}
  // copy
  Nuclide(const Nuclide&) = default;
  // move
  Nuclide(Nuclide&&) = default;

  // Assignment
  // copy
  Nuclide& operator=(const Nuclide&) = default;
  // move
  Nuclide& operator=(Nuclide&&) = default;

  // Destructors
  ~Nuclide() = default;

  static constexpr int NUBASE_START_A             = 0;
  static constexpr int NUBASE_END_A               = 3;
  static constexpr int NUBASE_START_Z             = 4;
  static constexpr int NUBASE_END_Z               = 7;
  static constexpr int NUBASE_START_STATE         = 7;
  static constexpr int NUBASE_END_STATE           = 8;
  static constexpr int NUBASE_START_ME            = 18;
  static constexpr int NUBASE_END_ME              = 29;
  static constexpr int NUBASE_START_DME           = 29;
  static constexpr int NUBASE_END_DME             = 38;
  static constexpr int NUBASE_START_ISOMER        = 39;
  static constexpr int NUBASE_END_ISOMER          = 46;
  static constexpr int NUBASE_START_DISOMER       = 48;
  static constexpr int NUBASE_END_DISOMER         = 56;
  static constexpr int NUBASE_START_HALFLIFEVALUE = 60;
  static constexpr int NUBASE_END_HALFLIFEVALUE   = 69;
  static constexpr int NUBASE_START_HALFLIFEUNIT  = 69;
  static constexpr int NUBASE_END_HALFLIFEUNIT    = 71;
  static constexpr int NUBASE_START_SPIN          = 79;
  static constexpr int NUBASE_END_SPIN            = 93;
  // After the 2003 table, the discovery
  // year was added, alterting the positions
  static constexpr int NUBASE_START_YEAR = 105;
  static constexpr int NUBASE_END_YEAR   = 109;
  // Let the 03 position be the odd-one-out and thus
  // have the slightly awkward name
  static constexpr int NUBASE_START_DECAYSTRING_03 = 106;
  static constexpr int NUBASE_START_DECAYSTRING    = 110;
  // The decay string goes to EOL, put here, commented,
  // to show that we haven't just forgotten about it.
  // static constexpr int NUBASE_END_DECAYSTRING=EOL;

  static constexpr int AME_START_A   = 16;
  static constexpr int AME_END_A     = 19;
  static constexpr int AME_START_Z   = 11;
  static constexpr int AME_END_Z     = 14;
  static constexpr int AME_START_ME  = 29;
  static constexpr int AME_END_ME    = 41;
  static constexpr int AME_START_DME = 42;
  static constexpr int AME_END_DME   = 53;

  mutable bool own = false;

  mutable int exp    = 0;
  mutable int A      = 0;
  mutable int Z      = 0;
  mutable int N      = 0;
  mutable int pi     = 0;
  mutable int pi_exp = 0;
  mutable int J_exp  = 0;
  mutable int J_tent = 0;
  mutable int rich   = 0;
  mutable int show   = 0;
  mutable int year   = 1900;

  mutable double NUBASE_ME  = 0.1;
  mutable double NUBASE_dME = 1.0e4;
  mutable double AME_ME     = 0.1;
  mutable double AME_dME    = 1.0e4;
  mutable double s_n        = 0.0;
  mutable double ds_n       = 0.0;
  mutable double s_2n       = 0.0;
  mutable double ds_2n      = 0.0;
  mutable double s_p        = 0.0;
  mutable double ds_p       = 0.0;
  mutable double s_2p       = 0.0;
  mutable double ds_2p      = 0.0;
  mutable double dV_pn      = 0.0;
  mutable double ddV_pn     = 0.0;
  mutable double hl         = 0.0;
  mutable double J          = 0.0;

  mutable std::string halflife_unit;
  mutable std::string symbol;
  mutable std::string decay;
  mutable std::string colour;
  mutable std::string full_data;

  struct State
  {
    State(const int _level, const double _energy, const double _error) : level(_level), energy(_energy), error(_error)
    {
    }

    int level{ 0 };
    double energy{ 0.0 };
    double error{ 0.0 };
  };
  std::vector<State> energy_levels;

  static const std::string& missingUnit();

  inline void setOwn(const bool val) const noexcept { own = val; }
  inline void setN() const noexcept { N = A - Z; }

  inline void setA() const { extractValue(full_data, NUBASE_START_A, NUBASE_END_A, A); }
  inline void setZ() const { extractValue(full_data, NUBASE_START_Z, NUBASE_END_Z, Z); }
  inline void setNubaseMassExcess() const { extractValue(full_data, NUBASE_START_ME, NUBASE_END_ME, NUBASE_ME); }
  inline void setNubaseMassExcessError() const
  {
    extractValue(full_data, NUBASE_START_DME, NUBASE_END_DME, NUBASE_dME);
  }
  inline void setYear() const { extractValue(full_data, NUBASE_START_YEAR, NUBASE_END_YEAR, year); }
  inline void setHalfLifeValue() const
  {
    extractValue(full_data, NUBASE_START_HALFLIFEVALUE, NUBASE_END_HALFLIFEVALUE, hl);
  }
  inline void setHalfLifeUnit() const
  {
    extractValue(full_data, NUBASE_START_HALFLIFEUNIT, NUBASE_END_HALFLIFEUNIT, halflife_unit);
  }

  inline void setSymbol(const std::string& _symbol) const noexcept { symbol = _symbol; }
  inline void setAMEMassExcess(const std::string& line) const { extractValue(line, AME_START_ME, AME_END_ME, AME_ME); }
  inline void setAMEMassExcessError(const std::string& line) const
  {
    extractValue(line, AME_START_DME, AME_END_DME, AME_dME);
  }

  void setExperimental() const;
  inline void setExperimental(const int val) const noexcept { exp = val; }

  inline void setState(int state) const { extractValue(full_data, NUBASE_START_STATE, NUBASE_END_STATE, state); }
  inline void setIsomerEnergy(double& energy) const
  {
    extractValue(full_data, NUBASE_START_ISOMER, NUBASE_END_ISOMER, energy);
  }
  inline void setIsomerEnergyError(double& error) const
  {
    extractValue(full_data, NUBASE_START_DISOMER, NUBASE_END_DISOMER, error);
  }


  template<typename Type>
  inline void extractValue(const std::string& line, const int start, const int end, Type& value) const
  {
    std::stringstream in(line.substr(start, end - start));
    in >> value;
  }

  void setSpinParity() const;
  void setIsomerData(std::vector<Nuclide>& nuc, const int state) const;
  void setHalfLife() const;
  void setSeparationEnergies(const std::vector<Nuclide>& nuc) const;
  void setDecayMode(std::vector<bool>& pnSide, const int table_year) const;
  void setNeutronOrProtonRich(const std::vector<bool>& pnSide) const;

  // Create a function to calculate the error on a value
  //
  // Template functions to do the maths
  template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  constexpr T squarer(T v) const noexcept
  {
    return v * v;
  }


  template<typename T, typename... Args, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  constexpr T squarer(T first, Args... args) const noexcept
  {
    return first * first + squarer(args...);
  }


  // The actual function that will be called with a variable number of arguments
  template<typename... Args>
  constexpr double errorQuadrature(Args... args) const
  {
    return std::sqrt(squarer(args...));
  }
};

#endif // NUCLIDE_HPP
