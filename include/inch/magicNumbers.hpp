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

#include <fstream>
#include <map>
#include <vector>


class MagicNumbers
{
public:
  MagicNumbers(int minZ, int maxZ, int minN, int maxN) :
      Zmin(minZ),
      Zmax(maxZ),
      Nmin(minN),
      Nmax(maxN),
      numbers{ 2, 8, 20, 28, 50, 82, 126 },
      values{
        { 0, 22, 0, 12 },    // 2
        { 1, 30, 0, 22 },    // 8
        { 7, 52, 1, 33 },    // 20
        { 16, 56, 7, 38 },   // 28
        { 45, 92, 19, 56 },  // 50
        { 81, 140, 43, 84 }, // 82
        { 0, 0, 76, 95 }     // Z<=118 so Z=126 magic number is not needed
      }
  {
    constructMap();
  }

  MagicNumbers(const MagicNumbers&)     = default;
  MagicNumbers(MagicNumbers&&) noexcept = default;

  /// Delete both due to const members
  MagicNumbers& operator=(const MagicNumbers&) = delete;
  MagicNumbers& operator=(MagicNumbers&&) = delete;

  ~MagicNumbers() noexcept = default;

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

  /// Store the min proton value used to create the chart
  const int Zmin;
  /// Store the max proton value used to create the chart
  const int Zmax;
  /// Store the min neutron value used to create the chart
  const int Nmin;
  /// Store the max neutron value used to create the chart
  const int Nmax;

  /// The min coordinate that the line can have
  const int min_val{ -1 };
  ///
  const int max_val{ 2 };

  /// Container for the actual magic numbers
  mutable std::vector<int> numbers;
  /// Container for the hand chosen, aesthetic, limits of the corresponding line in N and Z
  mutable std::vector<Number> values;
  /// Amalgamation of the numbers and values containers
  mutable std::map<int, Number> coords;

  /**
   * Save the state and set the line colour and thickness to use
   *
   * \param The stream to write to, i.e. the chart
   *
   * \return Nothing
   */
  inline void EPSSetup(std::ofstream& outFile) const noexcept
  {
    outFile << "\n%Magic Numbers\n"
            << "gs\n"
            << "black rgb\n"
            << "1 u div sl" << std::endl;
  }

  /**
   * The lines are drawn so return the postscript interpreter to it's previous state
   *
   * \param The stream to write to, i.e. the chart
   *
   * \return Nothing
   */
  inline void EPSTearDown(std::ostream& outFile) const noexcept { outFile << "gr" << std::endl; }

  /**
   * Write the EPS code that draws the proton line
   *
   * \param The stream to write to, i.e. the chart
   * \param The magic number to draw
   *
   * \return Nothing
   */
  void EPSWriteProtonNumber(std::ofstream& outFile, const int number) const;

  /**
   * Write the EPS code that draws the neutron line
   *
   * \param The stream to write to, i.e. the chart
   * \param The magic number to draw
   *
   * \return Nothing
   */
  void EPSWriteNeutronNumber(std::ofstream& outFile, const int number) const;

private:
  /**
   * Merge the containers with the magic numbers and their coordinates
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void constructMap() const;
};

#endif // MAGICNUMBERS_HPP
