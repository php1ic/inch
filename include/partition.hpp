#ifndef PARTITION_HPP
#define PARTITION_HPP

#include "nuclide.hpp"

#include <string>
#include <vector>
#include <memory>


class Partition
{
 public:
  //Constructors
  /// We use this value elsewhere in the code so don't want to std::move
  explicit Partition(const std::string &_scheme) // NOLINT (modernize-pass-by-value)
    :scheme{_scheme}
  {}
  //copy
  Partition(const Partition&) = default;
  //move
  Partition(Partition&&) = default;

  //Assignment
  //copy
  Partition& operator=(const Partition&) = default;
  //move
  Partition& operator=(Partition&&) = default;

  //Destructors
  ~Partition() = default;

  /// TODO: Make this a template on the value variable.
  ///       It's a double for 4 of the 5 uses and std::string for the other
  struct section
  {
    section(std::string _colour, const double _value, const bool _draw)
      : colour{std::move(_colour)}, value(_value), draw(_draw)
    {
    }

    section() : colour{"white"} {}

    std::string colour;
    double value = 0.0;
    bool draw {false};
  };
  std::vector<section> values;


  /// Sensible default values
  void setDefaultColours();
  void setDefaultMassExcessColours();
  void setDefaultRelativeErrorColours();
  void setDefaultDecayColours();
  void setDefaultHalfLifeColours();
  void setDefaultIsomerEnergyColours();

  /// Let the user define their own values
  void setUserColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserMassExcessColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserRelativeErrorColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserDecayColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserHalfLifeColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);
  void setUserIsomerEnergyColours(const std::vector<std::string>& _colours, const std::vector<double>& _values);

  /// Automatically partition in to n
  static constexpr int PARTS=5;
  /// By passing as a copy and NOT a reference we can rearrange the vector as required
  /// without the need for a 2nd sort to put it back to how it was.
  /// TODO: benchmark pass as a copy vs by reference and sorting twice.
  void setAutoColours(std::vector<Nuclide> theTable, const int parts=PARTS);
  /// Doesn't make sense to automatically partition decay mode.
  /// Putting here, but commented, to show it hasn't been forgotten
  //void setAutoDecayColours() {};
  void setAutoMassExcessColours(std::vector<Nuclide> theTable, const int parts=PARTS);
  void setAutoRelativeErrorColours(std::vector<Nuclide> theTable, const int parts=PARTS);
  void setAutoHalfLifeColours(std::vector<Nuclide> theTable, const int parts=PARTS);
  void setAutoIsomerEnergyColours(std::vector<Nuclide> theTable, const int parts=PARTS);

  /// Implement if we start passing the original vector by reference
  //void resetSort(std::vector<Nuclide> &theTable);

private:
  mutable std::string scheme;

  inline void setScheme(const std::string& _scheme) { scheme = _scheme; }

  inline void clearData()
  {
    values.clear();
    values.shrink_to_fit();
  }
};

#endif // PARTITION_HPP
