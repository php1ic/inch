/**
 * \class UI
 *
 * \brief A place for user interaction functionality
 */
#ifndef UI_HPP
#define UI_HPP

#include <string_view>

#include "inch/allNeutrons.hpp"
#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/chartType.hpp"
#include "inch/massTable.hpp"
#include "inch/nuclide.hpp"

#include <fmt/core.h>

#include <string>
#include <utility>
#include <vector>

class UI
{
public:
  explicit UI(Options& _options) : user_options(_options) {}

  UI(const UI& other)     = default;
  UI(UI&& other) noexcept = default;

  UI& operator=(const UI& other) = delete;
  UI& operator=(UI&& other) noexcept = delete;

  virtual ~UI() noexcept = default;

  /**
   * This is not yet implemented
   *
   * \param Nothing
   *
   * \return Nothing
   */
  // void getOptions();

  /**
   * Parent function that just runs other question functions
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void askQuestions(const MassTable& table) const;

private:
  /// Mass table data used to validate answers to questions
  // mutable std::vector<Nuclide> table;

  /// Which properties are being used to create the chart
  Options& user_options;

  /**
   * Ask <TheQuestion>, giving the user <theOptions> as possible answers.
   * Allow the user to not proivdes a valid answer <attempts> times before
   * using <fallback> as the answer.
   *
   * \param The question to ask
   * \param A vector of std::string containing the answer options
   * \param The default answer to used
   * \param The number of attempts the user get to answer the question correctly
   *
   * \return The index of the answer possibiliites to the question asked
   */
  [[nodiscard]] size_t genericQuestion(const std::string& theQuestion,
                                       const std::vector<std::string>& theOptions,
                                       const int fallback,
                                       const int attempts) const;

  /**
   * Should the chart display measured, theoretical or both types of isotopes
   *
   * \param Nothing
   *
   * \return Nothing
   */
  [[nodiscard]] ChartType selectChartType() const;

  /**
   * Which isotopic property should the chart be coloured by
   *
   * \param Nothing
   *
   * \return Nothing
   */
  [[nodiscard]] ChartColour selectChartColour() const;

  /**
   * How much of the chart should be drawn
   *
   * \param Nothing
   *
   * \return Nothing
   */
  [[nodiscard]] ChartSelection selectChartSelection() const;

  [[nodiscard]] AllNeutrons selectWhichNeutronRange() const;

  void selectZandNLimits(const ChartSelection selection, const MassTable& table) const;
};

#endif // UI_HPP
