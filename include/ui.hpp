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
  // Default constructor
  explicit UI(const std::vector<Nuclide>& _table, Options& _options) : table(_table), options(_options) {}

  // Copy
  UI(const UI& other) = default;
  // Move
  UI(UI&& other) noexcept = default;

  // Copy assignment
  UI& operator=(const UI& other) = delete;
  // Move assignment
  UI& operator=(UI&& other) noexcept = delete;

  // Destructor
  virtual ~UI() noexcept = default;

  void getOptions();
  void askQuestions();

private:
  mutable std::vector<Nuclide> table;

  Options& options;

  // Helper function, refactor and inherit?
  void setExtreme(const std::string& limit) const;

  void selectChartType() const;
  void selectChartColour() const;
  void selectChartSelection() const;
};

#endif // UI_HPP
