/**
 *
 * \class MagicNumbers
 *
 * \brief Encapsulation of the magic number lines
 *
 * All magic numbers are identical beyond their start and end points
 * wrap it up here
 *
 */
#ifndef MAGICNUMBERS_HPP
#define MAGICNUMBERS_HPP

#include "inch/limits.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>


class MagicNumbers
{
public:
  explicit MagicNumbers(Limits _limits) : limits(std::move(_limits)) { constructMap(); }

  MagicNumbers(const MagicNumbers&)     = default;
  MagicNumbers(MagicNumbers&&) noexcept = default;

  /// Delete both due to const members
  MagicNumbers& operator=(const MagicNumbers&) = delete;
  MagicNumbers& operator=(MagicNumbers&&) = delete;

  virtual ~MagicNumbers() noexcept = default;

  /**
   * \struct Number
   *
   * \brief The start and end points of the line
   */
  struct Number
  {
    Number(int minN, int maxN, int minZ, int maxZ) : min_n{ minN }, max_n{ maxN }, min_z{ minZ }, max_z{ maxZ } {}

    int min_n;
    int max_n;
    int min_z;
    int max_z;
  };

  /// Store the N,Z limits of the chart being created
  const Limits limits;

  /// The min coordinate that the line can have
  const int min_val{ -1 };
  /// The max coordinate that the line can have
  const int max_val{ 2 };

  /// Container for the actual magic numbers
  mutable std::vector<int> numbers{ 2, 8, 20, 28, 50, 82, 126 };
  /// Container for the hand chosen, aesthetic, limits of the corresponding line in N and Z
  mutable std::vector<Number> values{
    { 0, 22, 0, 12 },    // 2
    { 1, 30, 0, 22 },    // 8
    { 7, 52, 1, 33 },    // 20
    { 16, 56, 7, 38 },   // 28
    { 45, 92, 19, 56 },  // 50
    { 81, 140, 43, 84 }, // 82
    { 0, 0, 76, 95 }     // Z<=118 so Z=126 magic number is not needed
  };
  /// Amalgamation of the numbers and values containers
  mutable std::map<int, Number> coords;

  /**
   * Merge the containers with the magic numbers and their coordinates
   *
   * \param Nothing
   *
   * \return Nothing
   */
  inline void constructMap() const
  {
    std::transform(numbers.cbegin(),
                   numbers.cend(),
                   values.cbegin(),
                   std::inserter(coords, coords.end()),
                   [](const int a, const Number b) { return std::make_pair(a, b); });
  }

  /**
   * Save the state and set the line colour and thickness to use
   *
   * Nothing
   *
   * \return The string to write to file
   */
  [[nodiscard]] virtual std::string Setup() const = 0;

  /**
   * The lines are drawn so return the postscript interpreter to it's previous state
   *
   * \param Nothing
   *
   * \return The string to write to file
   */
  [[nodiscard]] virtual std::string TearDown() const = 0;

  /**
   * Write the EPS code that draws the proton line
   *
   * \param The stream to write to, i.e. the chart
   * \param The magic number to draw
   *
   * \return Nothing
   */
  [[nodiscard]] virtual std::string WriteProtonNumber(const int number) const = 0;

  /**
   * Write the EPS code that draws the neutron line
   *
   * \param The stream to write to, i.e. the chart
   * \param The magic number to draw
   *
   * \return Nothing
   */
  [[nodiscard]] virtual std::string WriteNeutronNumber(const int number) const = 0;
};

#endif // MAGICNUMBERS_HPP
