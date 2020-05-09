/**
 * \class UI
 *
 * \brief A place for user interaction functionality
 */
#ifndef UI_HPP
#define UI_HPP

#include <string_view>

#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/chartType.hpp"
#include "inch/nuclide.hpp"
#include "inch/options.hpp"

#include <memory>
#include <regex>
#include <string>
#include <utility>
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
  void askQuestions() const;

private:
  /// Mass table data used to validate answers to questions
  mutable std::vector<Nuclide> table;

  /// Which properties are being used to create the chart
  Options& options;

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
   * Using the currently set values of Zmin and Zmax, read the mass table
   * and find the assoicated Nmin and Mmax for each Z. Also find the N values
   * of first and last stable isotope for that Z
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void setUserNeutronRange() const;

  /**
   * Get the range of N values for the isotopes with <Z>. Filter on <decayMode> if required.
   * The filter is a regex so default is anything.
   *
   * \param The Z to get the N range for
   * \param The decay mode to filter on
   *
   * \return A std::pair<int,int> with min and max N
   */
  [[nodiscard]] std::pair<int, int> GetNeutronRange(const int Z, const std::string& decayMode = ".") const;

  /**
   * For the isotope with Z=<Z>, get the min and max N values of the stable isotopes.
   *
   * \param The Z value to get the stable N range for
   *
   * \return A std::pair<int,int> with min and max N
   */
  [[nodiscard]] inline std::pair<int, int> GetStableNeutronRange(const int Z) const
  {
    return GetNeutronRange(Z, "stable");
  }

  /**
   * For the isotope with Z=<Z>, depending on <limit>, extract the N range
   * and ask the user to select a value
   *
   * \param The Z value to set the N limit for
   * \param The limit to set (Nmin or Nmax)
   *
   * \return Nothing
   */
  void SetNeutronLimitForZ(const int Z, std::string_view limit) const;

  /**
   * Set the upper/lower limits on N or Z
   *
   * \param The value to set a limit on [Zmin,Zmax,Nmin,Nmax]
   *
   * \return Nothing
   */
  void setExtreme(std::string_view limit) const;

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
