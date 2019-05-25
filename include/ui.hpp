/**
 * \class UI
 *
 * \brief A place for user interaction functionality
 */
#ifndef UI_HPP
#define UI_HPP

#include "chartColour.hpp"
#include "chartSelection.hpp"
#include "chartType.hpp"
#include "nuclide.hpp"
#include "options.hpp"

#include <memory>
#include <string>
#include <vector>

class UI
{
public:
  explicit UI(std::vector<Nuclide> _table, Options& _options) : table(std::move(_table)), options(_options) {}

  UI(const UI& other)     = default;
  UI(UI&& other) noexcept = default;

  UI& operator=(const UI& other) = delete;
  UI& operator=(UI&& other) noexcept = delete;

  virtual ~UI() noexcept = default;

  /**
   * ?????
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void getOptions();

  /**
   * Parent function that just runs other question functions
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void askQuestions();

private:
  /// Mass table data used to validate answers to questions
  mutable std::vector<Nuclide> table;

  /// Which properties are being used to create the chart
  Options& options;

  /**
   * Set the upper/lower limits on N or Z
   *
   * \param The value to set a limit on [Zmin,Zmax,Nmin,Nmax]
   *
   * \return Nothing
   */
  void setExtreme(const std::string& limit) const;

  /**
   * Should the chart display measured, theoretical or both types of isotopes
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void selectChartType() const;

  /**
   * Which isotopic property should the chart be coloured by
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void selectChartColour() const;

  /**
   * How much of the chart should be drawn
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void selectChartSelection() const;
};

#endif // UI_HPP
