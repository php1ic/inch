/**
 *
 * \class Partition
 *
 * \brief Split the data that will be displayed in 'equal' partitions for colouring
 *
 * If the user selects a subset of isotopes to draw, they may all fall into the same global
 * partition so be coloured the same. This class provides a way to split any data range so
 * it can be differentiated as required.
 */
#ifndef PARTITION_HPP
#define PARTITION_HPP

#include "inch/chartColour.hpp"
#include "inch/nuclide.hpp"

#include <algorithm>
#include <string>
#include <vector>


class Partition
{
public:
  explicit Partition(const ChartColour& _scheme) : scheme{ _scheme } {}

  Partition(const Partition&) = default;
  Partition(Partition&&)      = default;

  Partition& operator=(const Partition&) = default;
  Partition& operator=(Partition&&) = default;

  ~Partition() = default;

  /**
   * \struct section
   *
   * \brief Container for easy access to parition boundaries
   *
   * TODO: Make this a template on the value variable.
   *      It's a double for 4 of the 5 uses and std::string for the other
   */
  struct section
  {
    section(std::string _colour, const double _value, const bool _draw) :
        colour{ std::move(_colour) }, value(_value), draw(_draw)
    {
    }

    std::string colour{ "white" };
    double value{ 0.0 };
    bool draw{ false };
  };
  /// Storage for all of the boundaries
  mutable std::vector<section> values;

  /**
   * Get the colour associated with the given <value>, if a <start> value is specified, skip the first <start>
   * partitions
   *
   * \param The value to get the colour for
   * \param The number of partitions to skip
   *
   * \return The colour associated with <value>
   */
  [[nodiscard]] inline std::string getColour(const double value, const int start = 0) const
  {
    auto val = std::find_if(
        std::next(values.begin(), start), values.end(), [value](const auto& s) -> bool { return (value <= s.value); });

    val->draw = true;
    return val->colour;
  }

  /**
   * Get the colour associated with the given string <value>, if a <start> value is specified, skip the first <start>
   * partitions
   *
   * \param The std::string value to get the colour for
   * \param The number of partitions to skip
   *
   * \return The colour associated with <value>
   */
  [[nodiscard]] inline std::string getColour(std::string_view value, const int start = 0) const
  {
    auto val = std::find_if(std::next(decayMap.cbegin(), start), decayMap.cend(), [&value](const auto& DM) -> bool {
      return (value == DM.first);
    });

    // Use numeric paired value to actually access the colour
    return getColour(val->second, start);
  }


  /**
   * Set default boundaries for all of the partitions
   *
   * I'm not going to make a new doxygen header for each function,
   * they are verbosely named
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void setDefaultColours();
  void setDefaultMassExcessColours();
  void setDefaultRelativeErrorColours();
  void setDefaultDecayColours();
  void setDefaultHalfLifeColours();
  void setDefaultIsomerEnergyColours();

  /**
   * User defined partitions
   *
   * \param The colours to use for each partition
   * \param The partition boundaries
   *
   * \return Nothing
   */
  void setUserColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserMassExcessColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserRelativeErrorColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserDecayColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserHalfLifeColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserIsomerEnergyColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);


  /**
   * Use an algorithm to set the boundaries on the data
   *
   * \param The mass table with all the data that will be displayed/partition
   * \param Number of partitions to create
   *
   * \return Nothing
   */
  /// Automatically partition in to n
  static constexpr int PARTS{ 5 };
  /// By passing as a copy and NOT a reference we can rearrange the vector as required
  /// without the need for a 2nd sort to put it back to how it was.
  /// TODO: benchmark pass as a copy vs by reference and sorting twice.
  void setAutoColours(std::vector<Nuclide> theTable, const int parts = PARTS);
  /// Doesn't make sense to automatically partition decay mode.
  /// Putting here, but commented, to show it hasn't been forgotten
  // void setAutoDecayColours() {};
  void setAutoMassExcessColours(std::vector<Nuclide> theTable, const int parts = PARTS);
  void setAutoRelativeErrorColours(std::vector<Nuclide> theTable, const int parts = PARTS);
  void setAutoHalfLifeColours(std::vector<Nuclide> theTable, const int parts = PARTS);
  void setAutoIsomerEnergyColours(std::vector<Nuclide> theTable, const int parts = PARTS);

  // Implement if we start passing the original vector by reference
  // void resetSort(std::vector<Nuclide> &theTable);

private:
  /// What property is the chart coloured by
  mutable ChartColour scheme;

  /// Set the property that the chart will be coloured by
  inline void setScheme(const ChartColour& _scheme) const { scheme = _scheme; }

  /// Enumerate the decay modes for easier algorithmic access
  mutable std::vector<std::pair<std::string, double>> decayMap{ { "stable", 0.0 },  { "A", 1.0 },  { "B-", 2.0 },
                                                                { "B+", 3.0 },      { "SF", 4.0 }, { "n", 5.0 },
                                                                { "2n", 6.0 },      { "p", 7.0 },  { "2p", 8.0 },
                                                                { "unknown", 9.0 }, { "EC", 10.0 } };

  /// Empty any previous boundary definitions that have been set
  inline void clearData()
  {
    values.clear();
    values.shrink_to_fit();
  }
};

#endif // PARTITION_HPP
