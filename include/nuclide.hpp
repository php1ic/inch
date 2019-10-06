/**
 *
 * \class Nuclide
 *
 * \brief Storage class for a single isotope
 *
 * There is a lot of data for each isotope, use this class to store, set and get
 * values associated with it
 */
#ifndef NUCLIDE_HPP
#define NUCLIDE_HPP

#include <cmath>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>


class Nuclide
{
public:
  explicit Nuclide(std::string line) : full_data(std::move(line)) {}

  Nuclide(const Nuclide&) = default;
  Nuclide(Nuclide&&)      = default;

  Nuclide& operator=(const Nuclide&) = default;
  Nuclide& operator=(Nuclide&&) = default;

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

  /// Is this isotope also present in the user file, so should be drawn differently
  mutable bool own = false;

  /// Is the isotope experimental or extrapolated/theoretical
  mutable int exp = 0;
  /// The mass number
  mutable int A = 0;
  /// The proton number
  mutable int Z = 0;
  /// The neutron number
  mutable int N = 0;
  /// The parity of the spin state
  mutable int pi = 0;
  /// Is the parity of the spin state experimental
  mutable int pi_exp = 0;
  /// The spin value of the state
  mutable int J_exp = 0;
  /// Is the spin value experimental
  mutable int J_tent = 0;
  /// Is the isotope neutron or proton rich
  /// (defined by which 'side' of stability it is on, not N=Z line)
  mutable int rich = 0;
  /// Should the isotope be drawn on the chart
  mutable int show = 0;
  /// What year was the isotope discovered
  mutable int year = 1900;

  /// Mass excess from the NUBASE table
  mutable double NUBASE_ME = 0.1;
  /// Error on the mass excess from the NUBASE table
  mutable double NUBASE_dME = 1.0e4;
  /// Mass excess from the AME table
  mutable double AME_ME = 0.1;
  /// Error on the mass excess from the AME table
  mutable double AME_dME = 1.0e4;
  /// One neutron separation energy
  mutable double s_n = 0.0;
  /// Error on the one neutron separation energy
  mutable double ds_n = 0.0;
  /// Two neutron separation energy
  mutable double s_2n = 0.0;
  /// Error on the two neutron separation energy
  mutable double ds_2n = 0.0;
  /// One proton separation energy
  mutable double s_p = 0.0;
  /// Error on the one proton separation energy
  mutable double ds_p = 0.0;
  /// Two proton separation energy
  mutable double s_2p = 0.0;
  /// Error on the two proton separtion energy
  mutable double ds_2p = 0.0;
  ///
  mutable double dV_pn = 0.0;
  ///
  mutable double ddV_pn = 0.0;
  /// Half life of the isotope
  mutable double hl = 0.0;
  /// Spin parity of the state
  mutable double J = 0.0;


  /// Human readable unit for half life
  mutable std::string halflife_unit;
  /// Isotopic symbol
  mutable std::string symbol;
  /// Decay mode of the isotope
  mutable std::string decay;
  /// What colour should the isotope be drawn on the chart
  mutable std::string colour;
  /// The entire line for the isotope from the data file
  mutable std::string full_data;

  /**
   * \struct State
   *
   * \brief Details additional energy levels of the isotope
   */
  struct State
  {
    State(const int _level, const double _energy, const double _error) : level(_level), energy(_energy), error(_error)
    {
    }

    int level{ 0 };
    double energy{ 0.0 };
    double error{ 0.0 };
  };
  /// Container for all levels that are recorded in the data file that is read
  std::vector<State> energy_levels;

  /**
   * Function to use if a value has no units
   *
   * \param Nothing
   *
   * \return std::string with contents "no_units"
   */
  static const std::string& missingUnit();

  /**
   * Set if the isotope is marked as user defined
   *
   * \param true/false for yes/no
   *
   * \return Nothing
   */
  inline void setOwn(const bool val) const noexcept { own = val; }

  /**
   * Set the neutron number
   *
   * \param Nothing
   *
   * \return Nothing
   */

  inline void setN() const noexcept { N = A - Z; }


  /**
   * Extract the mass number from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setA() const { extractValue(full_data, NUBASE_START_A, NUBASE_END_A, A); }

  /**
   * Extract the proton number from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setZ() const { extractValue(full_data, NUBASE_START_Z, NUBASE_END_Z, Z); }

  /**
   * Extract the mass-excess from the NUBASE data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setNubaseMassExcess() const { extractValue(full_data, NUBASE_START_ME, NUBASE_END_ME, NUBASE_ME); }

  /**
   * Extract the error on mass-excess from the NUBASE data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setNubaseMassExcessError() const
  {
    extractValue(full_data, NUBASE_START_DME, NUBASE_END_DME, NUBASE_dME);
  }

  /**
   * Extract the discovery year from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setYear() const { extractValue(full_data, NUBASE_START_YEAR, NUBASE_END_YEAR, year); }

  /**
   * Extract the half life value from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setHalfLifeValue() const
  {
    extractValue(full_data, NUBASE_START_HALFLIFEVALUE, NUBASE_END_HALFLIFEVALUE, hl);
  }

  /**
   * Extract the units of the half life value from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setHalfLifeUnit() const
  {
    extractValue(full_data, NUBASE_START_HALFLIFEUNIT, NUBASE_END_HALFLIFEUNIT, halflife_unit);
  }

  /**
   * Set the isotope symbol
   *
   * \param The std::string to use as the symbol
   *
   * \return Nothing
   */
  inline void setSymbol(std::string_view _symbol) const noexcept { symbol = _symbol; }

  /**
   * Extract the mass excess from a AME formatted line
   *
   * \param The line to extract the mass excess from
   *
   * \return Nothing
   */
  inline void setAMEMassExcess(const std::string& line) const { extractValue(line, AME_START_ME, AME_END_ME, AME_ME); }

  /**
   * Extract the error on the mass excess from a AME formatted line
   *
   * \param The line to extract the value from
   *
   * \return Nothing
   */
  inline void setAMEMassExcessError(const std::string& line) const
  {
    extractValue(line, AME_START_DME, AME_END_DME, AME_dME);
  }

  /**
   * Modify the line from the data file swapping '#' for ' ' to maintain positioning.
   * Also set the exp flag as required
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void setExperimental() const;

  /**
   * Set the experimental flag
   *
   * \param The value to set the experimental flag
   *
   * \return Nothing
   */
  inline void setExperimental(const int val) const noexcept { exp = val; }


  /**
   * Extract the state/level from the data file
   *
   * \param The value to be set (N.B. pass by reference)
   *
   * \return Nothing
   */
  inline void setState(int& state) const { extractValue(full_data, NUBASE_START_STATE, NUBASE_END_STATE, state); }

  /**
   * Extract the energy of the level from the data file
   *
   * \param The value to be set (N.B. pass by reference)
   *
   * \return Nothing
   */
  inline void setIsomerEnergy(double& energy) const
  {
    extractValue(full_data, NUBASE_START_ISOMER, NUBASE_END_ISOMER, energy);
  }

  /**
   * Extract the error on the energy of the state
   *
   * \param The value to be set (N.B. pass by reference)
   *
   * \return Nothing
   */
  inline void setIsomerEnergyError(double& error) const
  {
    extractValue(full_data, NUBASE_START_DISOMER, NUBASE_END_DISOMER, error);
  }

  /**
   * Template function to extract value of type <T> from a string
   *
   * \param The std::string containing the data to be extracted
   * \param Start position of the value
   * \param End position of the value
   * \param The variable that will be populated
   *
   * \return Nothing
   */
  template<typename Type>
  inline void extractValue(const std::string& line, const int start, const int end, Type& value) const
  {
    std::stringstream in(line.substr(start, end - start));
    in >> value;
  }

  /**
   * Extract the spin and parity from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void setSpinParity() const;

  /**
   * Extract the isomeric data; level, energy and error on energy
   *
   * \param The data read so far to allow us to populate the necessary state
   * \param The state we are getting information about
   *
   * \return Nothing
   */
  void setIsomerData(std::vector<Nuclide>& nuc, const int state) const;

  /**
   * Extract the half life from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void setHalfLife() const;

  /**
   * Calculate the separation energies for the isotope
   *
   * \param The data read so far so calculations can be done
   *
   * \return Nothing
   */
  void setSeparationEnergies(const std::vector<Nuclide>& nuc) const;

  /**
   * Extract the decay mode from the data file
   *
   * \param Container that keeps track of if we have read a stable isotope for this Z value
   * \param The year of the data file being read
   *
   * \return Nothing
   */
  void setDecayMode(std::vector<bool>& pnSide, const int table_year) const;

  /**
   * Set the rich flag for the isotope
   *
   * \param Container that keeps track of if we have read a stable isotope for this Z value
   *
   * \return Nothing
   */
  void setNeutronOrProtonRich(const std::vector<bool>& pnSide) const;

  /**
   * Function(s) to calculate the error on a value
   *
   * \param All of the numbres that need to be processed
   *
   * \return The numbers given, added in quadrature
   */
  template<typename... Args>
  constexpr double errorQuadrature(Args... args) const
  {
    return std::sqrt(squarer(args...));
  }

  template<typename T, typename... Args, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  constexpr T squarer(T first, Args... args) const noexcept
  {
    return first * first + squarer(args...);
  }

  template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  constexpr T squarer(T v) const noexcept
  {
    return v * v;
  }
};

#endif // NUCLIDE_HPP
