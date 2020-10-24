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
  explicit TIKZChart(Options& _options) : Chart(_options){};

  TIKZChart(const TIKZChart& TIKZChart)     = default;
  TIKZChart(TIKZChart&& TIKZChart) noexcept = default;

  TIKZChart& operator=(const TIKZChart& TIKZChart) = delete;
  TIKZChart& operator=(TIKZChart&& TIKZChart) noexcept = delete;

  ~TIKZChart() noexcept override = default;

  /// Container for the drip lines that will actually be drawn
  // mutable std::vector<TIKZDripLine> drip_lines;

  /**
   */
  void write(const std::vector<Nuclide>& massTable, const Partition& /*part*/) const override;

  [[nodiscard]] std::string prolog() const override;

  [[nodiscard]] std::string header() const override;

  [[nodiscard]] std::string definitions() const override { return std::string(); }

  [[nodiscard]] std::string colours() const override { return std::string(); }

  [[nodiscard]] std::string isotope() const override;

  [[nodiscard]] std::string infoComment() const override { return std::string(); }

  [[nodiscard]] std::string setup() const override;

  [[nodiscard]] std::string teardown() const override;

  [[nodiscard]] inline std::string KeySetup(const int /*ZRange*/) const override { return std::string(); }

  [[nodiscard]] inline std::string KeyTearDown() const override { return std::string(); }
};

#endif // TIKZCHART_HPP
