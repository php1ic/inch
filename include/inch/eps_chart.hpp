/**
 *
 * \class EPSChart
 *
 * \brief
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
  EPSChart(Options& _options) : Chart(_options){};

  EPSChart(const EPSChart& EPSChart)     = default;
  EPSChart(EPSChart&& EPSChart) noexcept = default;

  EPSChart& operator=(const EPSChart& EPSChart) = delete;
  EPSChart& operator=(EPSChart&& EPSChart) noexcept = delete;

  ~EPSChart() noexcept = default;

  /// Container for the drip lines that will actually be drawn
  mutable std::vector<EPSDripLine> drip_lines;

  /**
   */
  void write(const std::vector<Nuclide>& massTable, const Partition& part) const;

  std::string prolog() const;

  std::string setup() const;

  std::string teardown() const;

  std::string KeySetup(const int ZRange) const;

  std::string KeyTearDown() const;
};

#endif // EPSCHART_HPP
