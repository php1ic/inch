/**
 *
 * \class Key
 *
 * \brief Encapulated the creation of the chart's key
 *
 *
 *
 */
#ifndef KEY_HPP
#define KEY_HPP

#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>


class Partition;
class Options;

class Key
{
public:
  Key() = default;

  Key(const Key& Key) = default;
  Key(Key&& Key)      = default;

  Key& operator=(const Key& Key) = default;
  Key& operator=(Key&& Key) noexcept = default;

  ~Key() noexcept = default;

  /// The min Z range at which the chart is vertically centered.
  /// If the Z ragne is larger then this, the key is vertically centered
  static constexpr int KEY_YOFFSET = 9;

  /// How tall is the key
  mutable double height = 0.5;
  /// How large is a key element, relative to one from the chart
  mutable double scale  = 0.0;

  /// The text used within the key
  mutable std::vector<std::string> textStrings;

  /// Aesthetic values for the key position when the full chart is drawn.
  /// In all other cases the key is to the right of the chart
  mutable std::vector<std::pair<int, int>> fullChartKeyPosition = {
    { 15, 75 }, { 12, 81 }, { 9, 60 }, { 9, 75 }, { 15, 75 }
  };

  /**
   * Using details about the chart and how it will be drawn,
   * set how large the key will be
   *
   * \param draw An instance of the Options class
   * \param part An instance of the Partition class
   *
   * \return Nothing
   */
  void setScale(const Options& draw, const Partition& part) const;

  /**
   * Using details about the chart and how it will be drawn,
   * populate the text used to describe the isotope colours
   *
   * \param draw An instance of the Options class
   * \param part An instance of the Partition class
   *
   * \return Nothing
   */
  void EPSSetText(const Options& draw, const Partition& part) const;

  /**
   * Write the necessary functions used to create the chart into the chart
   *
   * \param outFile An ofstream to the file that is being created
   *
   * \return Nothing
   */
  void EPSSetup(std::ofstream& outFile) const;

  /**
   * Place the key onto the chart
   *
   * \param outFile An ofstream to the file that is being created
   * \param draw An instance of the Options class
   *
   * \return Nothing
   */
  void EPSPlaceKey(std::ofstream& outFile, const Options& draw) const;

  /**
   * Write adiitional function into the file that are not provided by EPSSetup()
   *
   * \param outFile An ofstream to the file that is being created
   * \param draw An instance of the Options class
   *
   * \return Nothing
   */
  void EPSAdditionalFunctions(std::ofstream& outFile, const Options& draw) const;

  /**
   * Write the elements of the key onto the chart
   *
   * \param outFile An ofstream to the file that is being created
   * \param part An instance of the Partition class
   *
   * \return Nothing
   */
  void EPSWrite(std::ofstream& outFile, const Partition& part) const;

  /**
   * Create a box to encompass the key
   *
   * \param outFile An ofstream to the file that is being created
   *
   * \return Nothing
   */
  void EPSSurroundingBox(std::ofstream& outFile) const;
};

#endif // KEY_HPP
