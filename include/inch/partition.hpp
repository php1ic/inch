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

  /// Empty any previous boundary definitions that have been set
  inline void clearData()
  {
    values.clear();
    values.shrink_to_fit();
  }
};

#endif // PARTITION_HPP
