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

#include <string_view>

#include "inch/converter.hpp"
#include "inch/isotopeDisplayMode.hpp"
#include "inch/options.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <string>
#include <utility>
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

  static constexpr int NUBASE_START_A{ 0 };
  static constexpr int NUBASE_END_A{ 3 };
  static constexpr int NUBASE_START_Z{ 4 };
  static constexpr int NUBASE_END_Z{ 7 };
  static constexpr int NUBASE_START_STATE{ 7 };
  static constexpr int NUBASE_END_STATE{ 8 };
  static constexpr int NUBASE_START_ME{ 18 };
  static constexpr int NUBASE_END_ME{ 29 };
  static constexpr int NUBASE_START_DME{ 29 };
  static constexpr int NUBASE_END_DME{ 38 };
  static constexpr int NUBASE_START_ISOMER{ 39 };
  static constexpr int NUBASE_END_ISOMER{ 46 };
  static constexpr int NUBASE_START_DISOMER{ 48 };
  static constexpr int NUBASE_END_DISOMER{ 56 };
  static constexpr int NUBASE_START_HALFLIFEVALUE{ 60 };
  static constexpr int NUBASE_END_HALFLIFEVALUE{ 69 };
  static constexpr int NUBASE_START_HALFLIFEUNIT{ 69 };
  static constexpr int NUBASE_END_HALFLIFEUNIT{ 71 };
  static constexpr int NUBASE_START_HALFLIFEERROR{ 72 };
  static constexpr int NUBASE_END_HALFLIFEERROR{ 77 };
  static constexpr int NUBASE_START_SPIN{ 79 };
  static constexpr int NUBASE_END_SPIN{ 93 };
  // After the 2003 table, the discovery
  // year was added, alterting the positions
  static constexpr int NUBASE_START_YEAR{ 105 };
  static constexpr int NUBASE_END_YEAR{ 109 };
  // Let the 03 position be the odd-one-out and thus
  // have the slightly awkward name
  static constexpr int NUBASE_START_DECAYSTRING_03{ 106 };
  static constexpr int NUBASE_START_DECAYSTRING{ 110 };
  // The decay string goes to EOL, put here, commented,
  // to show that we haven't just forgotten about it.
  // static constexpr int NUBASE_END_DECAYSTRING{EOL};

  static constexpr int AME_START_A{ 16 };
  static constexpr int AME_END_A{ 19 };
  static constexpr int AME_START_Z{ 11 };
  static constexpr int AME_END_Z{ 14 };
  static constexpr int AME_START_ME{ 29 };
  static constexpr int AME_END_ME{ 41 };
  static constexpr int AME_START_DME{ 42 };
  static constexpr int AME_END_DME{ 53 };

  /// Is this isotope also present in the user file, so should be drawn differently
  mutable bool own{ false };

  /// Is the isotope experimental or extrapolated/theoretical
  mutable int exp{ 0 };
  /// The mass number
  mutable int A{ 0 };
  /// The proton number
  mutable int Z{ 0 };
  /// The neutron number
  mutable int N{ 0 };
  /// The parity of the spin state
  mutable int pi{ 0 };
  /// Is the parity of the spin state experimental
  mutable int pi_exp{ 0 };
  /// The spin value of the state
  mutable int J_exp{ 0 };
  /// Is the spin value experimental
  mutable int J_tent{ 0 };
  /// Is the isotope neutron or proton rich
  /// (defined by which 'side' of stability it is on, not N=Z line)
  mutable int rich{ 0 };
  /// Should the isotope be drawn on the chart
  mutable int show{ 0 };
  /// What year was the isotope discovered
  mutable int year{ 1900 };

  /// Mass excess from the NUBASE table
  mutable double NUBASE_ME{ 0.1 };
  /// Error on the mass excess from the NUBASE table
  mutable double NUBASE_dME{ 1.0e4 };
  /// Mass excess from the AME table
  mutable double AME_ME{ 0.1 };
  /// Error on the mass excess from the AME table
  mutable double AME_dME{ 1.0e4 };
  /// One neutron separation energy
  mutable double s_n{ 0.0 };
  /// Error on the one neutron separation energy
  mutable double ds_n{ 0.0 };
  /// Two neutron separation energy
  mutable double s_2n{ 0.0 };
  /// Error on the two neutron separation energy
  mutable double ds_2n{ 0.0 };
  /// One proton separation energy
  mutable double s_p{ 0.0 };
  /// Error on the one proton separation energy
  mutable double ds_p{ 0.0 };
  /// Two proton separation energy
  mutable double s_2p{ 0.0 };
  /// Error on the two proton separtion energy
  mutable double ds_2p{ 0.0 };
  ///
  mutable double dV_pn{ 0.0 };
  ///
  mutable double ddV_pn{ 0.0 };
  /// Spin parity of the state
  mutable double J{ 0.0 };

  /// Half life of the isotope
  mutable std::chrono::duration<double> hl{};
  /// Error on the half life of the isotope
  mutable std::chrono::duration<double> hl_error{};

  /// Human readable unit for half life
  mutable std::string halflife_unit{};
  /// Isotopic symbol
  mutable std::string symbol{};
  /// Decay mode of the isotope
  mutable std::string decay{};
  /// What colour should the isotope be drawn on the chart
  mutable std::string colour{};
  /// The entire line for the isotope from the data file
  mutable std::string full_data{};

  /// Known value to use if lifetime has no units
  static inline const std::string noUnit = "no_units";

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
  mutable std::vector<State> energy_levels;

  /**
   * Calculate the relative error on the isotope, but use a low water mark of <min_allowed>
   *
   * \param AME Use NUBASE or AME data
   * \param min_allowed The value to not let the relative error fall below
   *
   * \return The relative error on the isotope or min_allowed, whichever is larger
   */
  [[nodiscard]] double getRelativeMassExcessError(const bool AME, const double min_allowed) const;

  /**
   * Check if the isotope is within the criteria defined by the options
   *
   * \param An instance of the Option class
   *
   * \return [true] If the isotope is in the range to be drawn
   * \return [false] If the isotope is NOT in the range to be drawn
   */
  [[nodiscard]] inline bool isShown(const Options& options) const
  {
    return (options.limits.inZRange(Z) && options.limits.inNRange(N) && exp != static_cast<int>(options.chart_type)
            && rich % options.np_rich == 0);
  }

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
  inline void setA() const { A = Converter::StringToInt(full_data, NUBASE_START_A, NUBASE_END_A); }

  /**
   * Extract the proton number from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setZ() const { Z = Converter::StringToInt(full_data, NUBASE_START_Z, NUBASE_END_Z); }

  /**
   * Extract the mass-excess from the NUBASE data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setNubaseMassExcess() const
  {
    NUBASE_ME = Converter::StringToDouble(full_data, NUBASE_START_ME, NUBASE_END_ME);
  }

  /**
   * Extract the error on mass-excess from the NUBASE data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setNubaseMassExcessError() const
  {
    NUBASE_dME = Converter::StringToDouble(full_data, NUBASE_START_DME, NUBASE_END_DME);
  }

  /**
   * Extract the discovery year from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setYear() const { year = Converter::StringToInt(full_data, NUBASE_START_YEAR, NUBASE_END_YEAR); }

  /**
   * Extract the half life value from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setHalfLifeValue() const
  {
    hl = Converter::seconds{ Converter::StringToDouble(
        full_data, NUBASE_START_HALFLIFEVALUE, NUBASE_END_HALFLIFEVALUE) };
  }

  /**
   * Extract the half life value from the data file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void setHalfLifeErrorValue() const
  {
    hl_error = Converter::seconds{ Converter::StringToDouble(
        full_data, NUBASE_START_HALFLIFEERROR, NUBASE_END_HALFLIFEERROR) };
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
    halflife_unit = full_data.substr(NUBASE_START_HALFLIFEUNIT, NUBASE_END_HALFLIFEUNIT - NUBASE_START_HALFLIFEUNIT);

    // Trim leading white space
    halflife_unit.erase(halflife_unit.begin(), std::find_if(halflife_unit.begin(), halflife_unit.end(), [](int ch) {
                          return (std::isspace(ch) == 0);
                        }));
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
  inline void setAMEMassExcess(const std::string& line) const
  {
    AME_ME = Converter::StringToDouble(line, AME_START_ME, AME_END_ME);
  }

  /**
   * Extract the error on the mass excess from a AME formatted line
   *
   * \param The line to extract the value from
   *
   * \return Nothing
   */
  inline void setAMEMassExcessError(const std::string& line) const
  {
    AME_dME = Converter::StringToDouble(line, AME_START_DME, AME_END_DME);
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
  inline void setState(int& state) const
  {
    state = Converter::StringToInt(full_data, NUBASE_START_STATE, NUBASE_END_STATE);
  }

  /**
   * Extract the energy of the level from the data file
   *
   * \param The value to be set (N.B. pass by reference)
   *
   * \return Nothing
   */
  inline void setIsomerEnergy(double& energy) const
  {
    energy = Converter::StringToDouble(full_data, NUBASE_START_ISOMER, NUBASE_END_ISOMER);
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
    error = Converter::StringToDouble(full_data, NUBASE_START_DISOMER, NUBASE_END_DISOMER);
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
  void setDecayMode(std::array<bool, Limits::MAX_Z + 1>& pnSide, const int table_year) const;

  /**
   * Set the rich flag for the isotope
   *
   * \param Container that keeps track of if we have read a stable isotope for this Z value
   *
   * \return Nothing
   */
  void setNeutronOrProtonRich(std::array<bool, Limits::MAX_Z + 1> const& pnSide) const;

  /**
   * Construct the string needed to display the isotope in an SVG file
   *
   * \param The minumum value of N in the chart
   * \param The maximum value of Z in the chart
   *
   * \return the string to write to the file
   */
  [[nodiscard]] inline std::string writeAsSVG(const int Nmin, const int Zmax) const
  {
    return fmt::format("<!--{}{}-->\n"
                       "<g transform=\"translate({} {})\"> <use xlink:href=\"#{}Nucleus\"/></g>\n",
                       A,
                       symbol,
                       (N - Nmin),
                       (Zmax - Z),
                       colour);
    //<< "<text class=\"MidSymbol Black\" dx=\"0.5\" dy=\"0.80\">" << it.symbol << "</text> "
    //<< "<text class=\"MidNumber Black\" dx=\"0.5\" dy=\"0.35\">" << it.A << "</text></g>" << std::endl;
  }

  /**
   * Construct the string needed to display the isotope in an TikZ file
   *
   * \param Nothing
   *
   * \return the string to write to the file
   */
  [[nodiscard]] inline std::string writeAsTIKZ() const
  {
    // Watch out for matching {}'s
    // You need {{ to print { in fmt, but also {N} for the nth thing to print
    return fmt::format("%{0}{1}\n"
                       "\\nucleus{{{2}}}{{{3}}}{{{4}}}{{{0}}}{{{1}}}\n",
                       A,
                       symbol,
                       colour,
                       N,
                       Z);
  }

  /**
   * Set how the shape representing the isotope is displayed
   *
   * \param The colour the chart will be coloured by
   *
   * \return The way that the isotope will be displayed
   */
  [[nodiscard]] inline IsotopeDisplayMode getDisplayMode(const ChartColour chart_colour) const
  {
    return (chart_colour == ChartColour::FIRST_ISOMERENERGY && decay == "stable") ? IsotopeDisplayMode::TopHalf
           : own                                                                  ? IsotopeDisplayMode::BottomLeftWedge
                                                                                  : IsotopeDisplayMode::EmptySquare;
  }

  /**
   * Output the colour to be used for writing the symbol and A value in the isotope shape
   *
   * \param The colour the chart is coloured by
   * \param Are we actually going to write the isotope text within the shape
   *
   * \return The text colour as a string. If we are not writing any text the string is empty (not NULL)
   */
  [[nodiscard]] inline std::string getIsotopeTextColour(const ChartColour chart_colour, const bool write_isotope) const
  {
    // If we are not writing the isotope then we should return an empty string
    std::string text_colour{};

    if (write_isotope)
      {
        // If the square is coloured black change the text to white
        // unless it a user isotope, in which case red
        text_colour = (colour == "black") ? own ? "red" : "white" : "black";

        // If we are colouring by isomer energy and the isotope is stable we draw a half black square
        // Use white text in this case
        // This is fragile!!
        // Any changes to how isomers of stable nuclei are displayed may need change here.
        if (chart_colour == ChartColour::FIRST_ISOMERENERGY && decay == "stable")
          {
            text_colour = "white";
          }
      }

    return text_colour;
  }

  /**
   * Construct the string needed to display the isotope in an EPS file
   *
   * \param An instance of the Option class
   *
   * \return the string to write to the file
   */
  [[nodiscard]] std::string writeAsEPS(const Options& draw) const;


  /**
   * Output all of the data as a csv string
   *
   * \param The string to separate values by [default = ","]
   *
   * \return All of the members separated by the given separator
   */
  std::string writeAsCSV(std::string_view sep = ",") const;

  /**
   * Output the header describing the string returned from Nuclide::writeAsCSV()
   *
   * \param Nothing
   *
   * \return A descriptive string of all member variables
   */
  static std::string CSVHeader();

  /**
   * Output all of the data as a json string
   *
   * \param Nothing
   *
   * \return All of the members in the format of a json ... unit
   */
  std::string writeAsJSON() const;

  /**
   * Function to calculate the error on a value via sum of squares
   *
   * \param All of the numbers that need to be processed
   *
   * \return The numbers given, added in quadrature then square rooted
   */
  template<typename... ARGS>
  static constexpr auto errorQuadrature(const ARGS&... args)
  {
    // Lots of brackets to ensure precedence and compilation
    return std::sqrt(((args * args) + ...));
  }
};

#endif // NUCLIDE_HPP
