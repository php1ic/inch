/**
 *
 * \class Key
 *
 * \brief Encapulated the creation of the chart's key
 *
 *
 *
 */
#ifndef EPSKEY_HPP
#define EPSKEY_HPP

#include "inch/chartColour.hpp"
#include "inch/key.hpp"
#include "inch/partition.hpp"

#include <fstream>
#include <string>
#include <utility>
#include <vector>


class Options;

class EPSKey : public BaseKey
{
public:
  EPSKey() = default;

  EPSKey(const EPSKey& EPSKey) = default;
  EPSKey(EPSKey&& EPSKey)      = default;

  EPSKey& operator=(const EPSKey& EPSKey) = default;
  EPSKey& operator=(EPSKey&& EPSKey) noexcept = default;

  ~EPSKey() noexcept = default;

  /// The min Z range at which the chart is vertically centered.
  /// If the Z ragne is larger then this, the key is vertically centered
  static constexpr int KEY_YOFFSET{ 9 };

  /// How tall is the key
  mutable double height{ 0.5 };
  /// How large is a key element, relative to one from the chart
  mutable double scale{ 0.0 };
  /// Don't let the key items get larger than this value
  mutable double max_scale{ 3.0 };
  /// Spce required to display a single item
  mutable double single_partition_height{ 1.5 };

  /// Aesthetic values for the key position when the full chart is drawn.
  /// In all other cases the key is to the right of the chart
  mutable std::vector<std::pair<int, int>> fullChartKeyPosition = {
    { 15, 75 }, { 12, 81 }, { 9, 60 }, { 9, 75 }, { 15, 75 }
  };


  /**
   * Using details about the chart and how it will be drawn,
   * populate the text used to describe the isotope colours
   *
   * \param part An instance of the Partition class
   *
   * \return Nothing
   */
  inline void SetText(const Partition& part) const { part.populateEPSKeyText(); }

  /**
   * Write the necessary functions used to create the chart into the chart
   *
   * \param outFile An ofstream to the file that is being created
   *
   * \return Nothing
   */
  std::string Setup() const;

  /**
   * Write the addition EPS function needed to display Mass Excess values
   *
   * \param Nothing
   *
   * \return the string to write in the file
   */
  std::string MassExcessSetup() const;

  /**
   * Write the addition EPS function needed to display half life values
   *
   * \param Nothing
   *
   * \return the string to write in the file
   */
  std::string HalLifeSetup() const;

  /**
   * Place the key onto the chart
   *
   * \param outFile An ofstream to the file that is being created
   * \param draw An instance of the Options class
   *
   * \return Nothing
   */
  std::string PlaceKey(const Options& draw) const;

  /**
   * Write additional function into the file that are not provided by EPSSetup()
   *
   * \param outFile An ofstream to the file that is being created
   * \param draw An instance of the Options class
   *
   * \return Nothing
   */
  std::string AdditionalFunctions(const ChartColour& colour) const;

  /**
   * Write the elements of the key onto the chart
   *
   * \param outFile An ofstream to the file that is being created
   * \param part An instance of the Partition class
   *
   * \return Nothing
   */
  void Write(std::ofstream& outFile, const Partition& part) const;

  /**
   * Create a box to encompass the key
   *
   * \param outFile An ofstream to the file that is being created
   *
   * \return Nothing
   */
  std::string SurroundingBox() const;
};

#endif // EPSKEY_HPP
