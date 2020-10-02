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

  /// Which properties are being used to create the chart
  Options& user_options;

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
  [[nodiscard]] static size_t genericQuestion(const std::string& theQuestion,
                                              const std::vector<std::string>& theOptions,
                                              const int fallback,
                                              const int attempts);

  /**
   * Ask the user <question>, which requires a y/n response.
   * Allow <attempts> non y or n inputs before using <fallback> as the response.
   *
   * \param The question to ask
   * \param The fallback response
   * \param The number of times the user can answer incorrectly before using the fallback
   *
   * \return [true] A reponse of yes
   * \return [false] A response of no
   */
  [[nodiscard]] static bool
  yesNoQuestion(const std::string& question, const std::string& fallback, const int attempts = 5);

  /**
   * Should the chart display measured, theoretical or both types of isotopes
   *
   * \param Nothing
   *
   * \return The chart type that will be displayed
   */
  [[nodiscard]] ChartType selectChartType() const;

  /**
   * Which isotopic property should the chart be coloured by
   *
   * \param Nothing
   *
   * \return The variable by with the chart will be coloured
   */
  [[nodiscard]] ChartColour selectChartColour() const;

  /**
   * How much of the chart should be drawn
   *
   * \param Nothing
   *
   * \return The snum to say if we are drawing all or some of the chart
   */
  [[nodiscard]] ChartSelection selectChartSelection() const;

  /**
   * How much of the chart should be drawn
   *
   * \param Nothing
   *
   * \return The enum saying if we are drawing all relevant neutron numbers
   */
  [[nodiscard]] AllNeutrons selectWhichNeutronRange() const;

  /**
   * Set the maxima values on N and Z
   *
   * \param If the all or just some of the chart is to be drawn
   * \param An instance of the mass table so we know N and Z ranges
   *
   * \return Nothing
   */
  void selectZandNLimits(const ChartSelection selection, const MassTable& table) const;
};

#endif // UI_HPP
