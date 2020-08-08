/**
 *
 * \class EPSChart
 *
 * \brief
 */

#ifndef TIKZCHART_HPP
#define TIKZCHART_HPP

#include "inch/chart.hpp"
//#include "inch/tikz_dripline.hpp"

#include <vector>

class Nuclide;
class Options;

class TIKZChart : public Chart
{
public:
  TIKZChart(Options& _options) : Chart(_options){};

  TIKZChart(const TIKZChart& TIKZChart)     = default;
  TIKZChart(TIKZChart&& TIKZChart) noexcept = default;

  TIKZChart& operator=(const TIKZChart& TIKZChart) = delete;
  TIKZChart& operator=(TIKZChart&& TIKZChart) noexcept = delete;

  ~TIKZChart() noexcept = default;

  /// Container for the drip lines that will actually be drawn
  // mutable std::vector<TIKZDripLine> drip_lines;

  /**
   */
  void write(const std::vector<Nuclide>& massTable, const Partition& part) const;

  std::string prolog() const;

  std::string setup() const;

  std::string teardown() const;

  std::string KeySetup(const int ZRange) const;

  std::string KeyTearDown() const;
};

#endif // TIKZCHART_HPP
