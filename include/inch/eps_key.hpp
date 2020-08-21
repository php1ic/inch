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

class EPSKey : public Key
{
public:
  EPSKey() = default;

  EPSKey(const EPSKey& EPSKey) = default;
  EPSKey(EPSKey&& EPSKey)      = default;

  EPSKey& operator=(const EPSKey& EPSKey) = default;
  EPSKey& operator=(EPSKey&& EPSKey) noexcept = default;

  ~EPSKey() noexcept override = default;

  /**
   * Using details about the chart and how it will be drawn,
   * populate the text used to describe the isotope colours
   *
   * \param part An instance of the Partition class
   *
   * \return Nothing
   */
  inline void SetText(const Partition& part) const override { part.populateEPSKeyText(); }

  /**
   * Write the necessary functions used to create the chart into the chart
   *
   * \param outFile An ofstream to the file that is being created
   *
   * \return Nothing
   */
  std::string Setup() const override;

  /**
   * Write the addition EPS function needed to display Mass Excess values
   *
   * \param Nothing
   *
   * \return the string to write in the file
   */
  std::string MassExcessSetup() const override;

  /**
   * Write the addition EPS function needed to display half life values
   *
   * \param Nothing
   *
   * \return the string to write in the file
   */
  std::string HalLifeSetup() const override;

  /**
   * Place the key onto the chart
   *
   * \param outFile An ofstream to the file that is being created
   * \param draw An instance of the Options class
   *
   * \return Nothing
   */
  std::string PlaceKey(const Options& draw) const override;

  /**
   * Write additional function into the file that are not provided by EPSSetup()
   *
   * \param outFile An ofstream to the file that is being created
   * \param draw An instance of the Options class
   *
   * \return Nothing
   */
  std::string AdditionalFunctions(const ChartColour& colour) const override;

  /**
   * Write the elements of the key onto the chart
   *
   * \param outFile An ofstream to the file that is being created
   * \param part An instance of the Partition class
   *
   * \return Nothing
   */
  void Write(std::ofstream& outFile, const Partition& part) const override;

  /**
   * Create a box to encompass the key
   *
   * \param outFile An ofstream to the file that is being created
   *
   * \return Nothing
   */
  std::string SurroundingBox() const override;
};

#endif // EPSKEY_HPP
