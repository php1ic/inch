/**
 *
 * \class EPSChart
 *
 * \brief Derived class to create the chart in EPS format
 *
 * Implement the purely virtual methods from the base
 */

#ifndef EPSCHART_HPP
#define EPSCHART_HPP

#include "inch/chart.hpp"
#include "inch/eps_dripline.hpp"

#include <vector>

class Nuclide;
class Options;

class EPSChart : public Chart
{
public:
  explicit EPSChart(Options& _options) : Chart(_options){};

  EPSChart(const EPSChart& EPSChart)     = default;
  EPSChart(EPSChart&& EPSChart) noexcept = default;

  EPSChart& operator=(const EPSChart& EPSChart)     = delete;
  EPSChart& operator=(EPSChart&& EPSChart) noexcept = delete;

  ~EPSChart() noexcept override = default;

  /// Container for the drip lines that will actually be drawn
  mutable std::vector<EPSDripLine> drip_lines;

  void write(const std::vector<Nuclide>& nuc, const Partition& part) const override;

  [[nodiscard]] std::string prolog() const override;

  [[nodiscard]] std::string header() const override;

  [[nodiscard]] std::string definitions() const override;

  [[nodiscard]] std::string colours() const override;

  [[nodiscard]] std::string isotope() const override;

  [[nodiscard]] std::string infoComment() const override;

  [[nodiscard]] std::string setup() const override;

  [[nodiscard]] std::string teardown() const override;

  [[nodiscard]] std::string KeySetup(const int ZRange) const override;

  [[nodiscard]] std::string KeyTearDown() const override;
};

#endif // EPSCHART_HPP
